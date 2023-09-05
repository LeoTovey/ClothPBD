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

// template <typename T>
// struct Spring
// {
//     /* data */
//     unsigned int node_0;
//     unsigned int node_1;
//     T rest_length;

//     Spring(unsigned int node_0, unsigned int node_1, T rest_length)
//         : node_0(node_0), node_1(node_1), rest_length(rest_length)
//     {
//     }
// };

template <typename T>
class MassSpringSim : public SimBase<T>
{
protected:
    // using SpringVec = std::vector<Spring<T>>;
    using Vec3I = SimBase<T>::Vec3I;
    using Vec3T = SimBase<T>::Vec3T;
    using PairVec = std::vector<std::pair<unsigned int, unsigned int>>;
    using SimBase<T>::n_particles;
    using SimBase<T>::n_edges;
    using SimBase<T>::x;
    using SimBase<T>::v;
    using SimBase<T>::f;
    using SimBase<T>::e;
    using SimBase<T>::l;
    using SimBase<T>::fixed_node_0;
    using SimBase<T>::fixed_node_1;

    float k = 1.0f;
    float mass = 1.0f;
    float damping = 1.0f;
    float gravity = 0.98;

public:
    MassSpringSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, MassSpringParams<T> params);

    void Substep(T dt) override;
    void Reset() override;
private:
    void Advect(T dt);
    void AddForces(T dt);
    void Collision(T dt);

};

template <typename T>
inline MassSpringSim<T>::MassSpringSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, MassSpringParams<T> params) : SimBase<T>(pos.size(), edges.size() / 2, pos, edges), k(params.k), mass(params.mass), damping(params.damping), gravity(params.gravity)
{

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
    Vec3T g = Vec3T(0.0, -gravity * mass, 0.0);
    std::for_each(f.begin(), f.end(), [g](Vec3T &force)
                  { force = g; });
    for (size_t i = 0; i < n_edges; i++)
    {
        unsigned int node_0 = e[i * 2 + 0];
        unsigned int node_1 = e[i * 2 + 1];

        Vec3T p0 = x[node_0];
        Vec3T p1 = x[node_1];

        T current_length = (p0 - p1).norm();
        Vec3T force_dir = (p0 - p1).normalized();

        Vec3T force = -k * (current_length - l[i]) * force_dir;

        f[node_0] += force;
        f[node_1] -= force;

        // damping
        Vec3T v_ij = v[node_0] - v[node_1];
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
        Vec3T collision_pos;
        if (this->sphere.IsColliding(x[i], collision_pos))
        {
            v[i] += (collision_pos - x[i]) / dt;
            x[i] = collision_pos;
        }
    }
}
