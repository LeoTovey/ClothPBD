#pragma once

#include "sim_base.h"

template <typename T>
struct JacobiSimParams
{
    T damping;
    T gravity;
    T k;
    T mass;
    int max_iteration;
};

template <typename T>
class JacobiSim : public SimBase<T>
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

    // for solving system of linear equations
    Eigen::VectorX<T> x_;
    Eigen::VectorX<T> v_;
    Eigen::VectorX<T> f_;
    Eigen::VectorX<T> h_;
    Eigen::VectorX<T> x_hat_;
    Eigen::VectorX<T> x_last_;
    Eigen::VectorX<T> g_;

    T damping;
    T gravity;
    T spirng_k;
    T mass;
    int max_iteration;

public:
    JacobiSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, JacobiSimParams<T> params);
    void Substep(T dt) override;
    void Reset() override;

    void JocobiSolve(T dt);
};

template <typename T>
inline JacobiSim<T>::JacobiSim(const std::vector<Vec3T> &pos, const std::vector<int> &edges, JacobiSimParams<T> params)
    : SimBase<T>(pos.size(), edges.size() / 2, pos, edges),
      spirng_k(params.k),
      mass(params.mass),
      damping(params.damping),
      gravity(params.gravity),
      max_iteration(params.max_iteration),
      x_(n_particles * 3),
      v_(n_particles * 3),
      f_(n_particles * 3),
      h_(n_particles * 3),
      x_hat_(n_particles * 3),
      x_last_(n_particles * 3),
      g_(n_particles * 3)
{
    for (size_t i = 0; i < n_particles; i++)
    {
        const Vec3T& p = x[i];
        x_[i * 3 + 0] = p[0];
        x_[i * 3 + 1] = p[1];
        x_[i * 3 + 2] = p[2];
        g_[i * 3 + 1] = gravity;
    }
    
}

template <typename T>
inline void JacobiSim<T>::Substep(T dt)
{
    T inv_dt = 1.0 / dt;
    T inv_dt_2 = 1.0 / (dt * dt);

    const Vec3T& fixed_p0 = x[fixed_node_0];
    const Vec3T& fixed_p1 = x[fixed_node_1];

    x_[fixed_node_0 * 3 + 0] = fixed_p0[0];
    x_[fixed_node_0 * 3 + 1] = fixed_p0[1];
    x_[fixed_node_0 * 3 + 2] = fixed_p0[2];
    x_[fixed_node_1 * 3 + 0] = fixed_p1[0];
    x_[fixed_node_1 * 3 + 1] = fixed_p1[1];
    x_[fixed_node_1 * 3 + 2] = fixed_p1[2];

    // advect
    x_last_ = x_;
    v_ *= damping;
    x_hat_ = x_ + dt * v_;
    x_ = x_hat_;

    // iterate
    for (size_t n = 0; n < max_iteration; n++)
    {
        h_ = mass * inv_dt_2 * (x_ - x_hat_) - mass * g_;

        for (size_t i = 0; i < n_edges; i++)
        {
            unsigned int node_0 = e[i * 2 + 0];
            unsigned int node_1 = e[i * 2 + 1];

            Vec3T x_i(x_[node_0 * 3 + 0], x_[node_0 * 3 + 1], x_[node_0 * 3 + 2]);
            Vec3T x_j(x_[node_1 * 3 + 0], x_[node_1 * 3 + 1], x_[node_1 * 3 + 2]);
            Vec3T x_ij = x_i - x_j;
            T x_ij_norm = x_ij.norm();

            Vec3T g = spirng_k * (1.0 - l[i] / x_ij_norm) * x_ij;
            h_[node_0 * 3 + 0] += g[0];
            h_[node_0 * 3 + 1] += g[1];
            h_[node_0 * 3 + 2] += g[2];
            h_[node_1 * 3 + 0] -= g[0];
            h_[node_1 * 3 + 1] -= g[1];
            h_[node_1 * 3 + 2] -= g[2];
        }

        x_ -= (1.0 / (mass * inv_dt_2 + 6.0 * spirng_k)) * h_;
    }

    v_ = inv_dt * (x_ - x_last_);

    // collsion TODO: 向量化处理
    for (size_t i = 0; i < n_particles; i++)
    {
        Vec3T collision_pos;
        Vec3T x_p(x_[i * 3 + 0], x_[i * 3 + 1], x_[i * 3 + 2]);
        if (this->sphere.IsColliding(x_p, collision_pos))
        {
            Vec3T delta_v = (collision_pos - x_p) * inv_dt;
            v_[i * 3 + 0] += delta_v[0];
            v_[i * 3 + 1] += delta_v[1];
            v_[i * 3 + 2] += delta_v[2];
            x_[i * 3 + 0] = collision_pos[0];
            x_[i * 3 + 1] = collision_pos[1];
            x_[i * 3 + 2] = collision_pos[2];
        }
    }

    x_[fixed_node_0 * 3 + 0] = x_last_[fixed_node_0 * 3 + 0];
    x_[fixed_node_0 * 3 + 1] = x_last_[fixed_node_0 * 3 + 1];
    x_[fixed_node_0 * 3 + 2] = x_last_[fixed_node_0 * 3 + 2];
    x_[fixed_node_1 * 3 + 0] = x_last_[fixed_node_1 * 3 + 0];
    x_[fixed_node_1 * 3 + 1] = x_last_[fixed_node_1 * 3 + 1];
    x_[fixed_node_1 * 3 + 2] = x_last_[fixed_node_1 * 3 + 2];

    for (size_t i = 0; i < n_particles; i++)
    {
        x[i] = Vec3T(x_[i * 3 + 0], x_[i * 3 + 1], x_[i * 3 + 2]);
    }
        
}

template <typename T>
inline void JacobiSim<T>::Reset()
{
}

template <typename T>
inline void JacobiSim<T>::JocobiSolve(T dt)
{
    // construct equations
    int n = n_particles;
    Eigen::SparseMatrix<T> A(3 * n, 3 * n);
    Eigen::VectorX<T> delta_x(3 * n);
    Eigen::VectorX<T> b(3 * n);

}
