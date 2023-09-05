#pragma once

#include "sim_base.h"
template <typename T>
struct MassSpringParams
{
    T damping;
    T gravity;
    T k;
    T mass;
};

template <typename T>
struct Spring
{
    /* data */
    unsigned int node_0;
    unsigned int node_1;
    T rest_length;

    Spring(unsigned int node_0, unsigned int node_1, T rest_length)
        : node_0(node_0), node_1(node_1), rest_length(rest_length)
    {
    }
};

template <typename T>
class MassSpringSim : public SimBase<T>
{
protected:
    using SpringVec = std::vector<Spring<T>>;
    using TV = SimBase<T>::TV;
    using V = SimBase<T>::V;
    using PairVec = std::vector<std::pair<unsigned int, unsigned int>>;
    using SimBase<T>::n_particles;
    using SimBase<T>::x;
    using SimBase<T>::v;
    using SimBase<T>::f;
    using SimBase<T>::fixed_node_0;
    using SimBase<T>::fixed_node_1;

    SpringVec springs;

    float k = 1.0f;
    float mass = 1.0f;
    float damping = 1.0f;
    float gravity = 0.98;

public:

    MassSpringSim(const TV &pos, const PairVec &indices, MassSpringParams<T> params);

    void Substep(T dt) override;
    void Reset() override;
private:
    void Advect(T dt);
    void AddForces(T dt);
    void Collision(T dt);

};

template <typename T>
inline MassSpringSim<T>::MassSpringSim(const TV &pos, const PairVec &indices, MassSpringParams<T> params) : SimBase<T>(pos.size()),
k(params.k), mass(params.mass), damping(params.damping), gravity(params.gravity)
{
    std::copy(pos.begin(), pos.end(), x.begin());
    for (const auto &[node_0, node_1] : indices)
    {
        const V &pos_0 = pos[node_0];
        const V &pos_1 = pos[node_1];

        T rest_length = (pos_0 - pos_1).norm();
        springs.emplace_back(node_0, node_1, rest_length);
    }
}

template <typename T>
inline void MassSpringSim<T>::Substep(T dt)
{
    // test
    Advect(dt);
    AddForces(dt);
    Collision(dt);
}

template <typename T>
inline void MassSpringSim<T>::Reset()
{
}

template <typename T>
inline void MassSpringSim<T>::Advect(T dt)
{
    for (size_t i = 0; i < n_particles; i++)
    {
        if (i != fixed_node_0 && i != fixed_node_1)
        {
            v[i] += f[i] / mass;
            x[i] += dt * v[i];
        }
    }
}

template <typename T>
inline void MassSpringSim<T>::AddForces(T dt)
{
    V g = V(0.0, - gravity * mass, 0.0);
    std::for_each(f.begin(), f.end(), [g](V &force){ force = g; });

    for (const auto &spring : springs)
    {
        unsigned int node_0 = spring.node_0;
        unsigned int node_1 = spring.node_1;
        V p0 = x[node_0];
        V p1 = x[node_1];

        T current_length = (p0 - p1).norm();
        V force_dir = (p0 - p1).normalized();

        V force = -k * (current_length - spring.rest_length) * force_dir;

        f[node_0] += force;
        f[node_1] -= force;

        // damping
        V v_ij = v[node_0] - v[node_1];
        T v_rel = v_ij.dot(force_dir);
        f[node_0] -= v_rel * damping * force_dir;
        f[node_1] += v_rel * damping * force_dir;
    }
}

template <typename T>
inline void MassSpringSim<T>::Collision(T dt)
{
    for (size_t i = 0; i < n_particles; i++)
    {
        V collision_pos;
        if (this->sphere.IsColliding(x[i], collision_pos))
        {
            v[i] += (collision_pos - x[i]) / dt;
            x[i] = collision_pos;
        }
    }
}
