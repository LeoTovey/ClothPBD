#pragma once

#include "sim_base.h"

template <typename T>
struct ImplicitSimParams
{
    T damping;
    T gravity;
    T k;
    T mass;
    int max_iteration;
};

template <typename T>
class ImplicitSim : public SimBase<T>
{
private:
    using Vec3I = SimBase<T>::Vec3I;
    using Vec3T = SimBase<T>::Vec3T;
    using SimBase<T>::n_particles;
    using SimBase<T>::n_edges;
    using SimBase<T>::x;
    using SimBase<T>::v;
    using SimBase<T>::f;

    using SimBase<T>::e;
    using SimBase<T>::l;

    using SimBase<T>::fixed_node_0;
    using SimBase<T>::fixed_node_1;

    // implicit solving
    std::vector<Vec3T> h;
    std::vector<Vec3T> x_hat;
    std::vector<Vec3T> x_last;

    T damping;
    T gravity;
    T spirng_k;
    T mass;
    int max_iteration;

public:
    ImplicitSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, ImplicitSimParams<T> params);
    void Substep(T dt) override;
    void Reset() override;
};

template <typename T>
inline ImplicitSim<T>::ImplicitSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, ImplicitSimParams<T> params) : SimBase<T>(pos.size(), edges.size() / 2, pos, edges), spirng_k(params.k), mass(params.mass), damping(params.damping), gravity(params.gravity), max_iteration(params.max_iteration)
{
    h.resize(n_particles);
    x_hat.resize(n_particles);
    x_last.resize(n_particles);
}

template <typename T>
inline void ImplicitSim<T>::Substep(T dt)
{
    // advect
    for (size_t i = 0; i < n_particles; i++)
    {
        x_last[i] = x[i];
        v[i] *= damping;
        x_hat[i] = x[i] + dt * v[i];
        x[i] = x_hat[i];
    }

    // iterate
    for (size_t n = 0; n < max_iteration; n++)
    {
        for (size_t i = 0; i < n_particles; i++)
        {
            h[i] = mass * (x[i] - x_hat[i]) / (dt * dt) - mass * Vec3T(0.0, gravity, 0.0);
        }

        for (size_t i = 0; i < n_edges; i++)
        {
            unsigned int node_0 = e[i * 2 + 0];
            unsigned int node_1 = e[i * 2 + 1];

            const Vec3T &x_i = x[node_0];
            const Vec3T &x_j = x[node_1];
            Vec3T x_ij = x_i - x_j;
            T x_ij_norm = x_ij.norm();

            Vec3T g = spirng_k * (1.0 - l[i] / x_ij_norm) * x_ij;
            h[node_0] += g;
            h[node_1] -= g;
        }

        T err = 0.0;
        for (size_t i = 0; i < n_particles; i++)
        {
            Vec3T delta_x = (1.0 / (mass / (dt * dt) + 6.0 * spirng_k)) * h[i];
            x[i] -= delta_x;
            err += delta_x.norm();
        }
        this->logs.push_back(std::to_string(err));
    }

    for (size_t i = 0; i < n_particles; i++)
    {
        v[i] = (x[i] - x_last[i]) / dt;
    }

    // collsion
    for (size_t i = 0; i < n_particles; i++)
    {
        Vec3T collision_pos;
        if (this->sphere.IsColliding(x[i], collision_pos))
        {
            v[i] += (collision_pos - x[i]) / dt;
            x[i] = collision_pos;
        }
    }

    x[fixed_node_0] = x_last[fixed_node_0];
    x[fixed_node_1] = x_last[fixed_node_1];
}

template <typename T>
inline void ImplicitSim<T>::Reset()
{
}
