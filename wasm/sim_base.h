#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <vector>
#include <math.h>

template <class T, int rows, int cols>
using Matrix = Eigen::Matrix<T, rows, cols>;

template <class T, int rows>
using Vector = Eigen::Matrix<T, rows, 1>;

template <class T>
using Vector3 = Vector<T, 3>;

template <typename T>
class CollisionShape
{
public:
    virtual bool IsColliding(const Vector3<T> &particle, Vector3<T> &result) const = 0;
};

template <typename T>
class Sphere : CollisionShape<T>
{
public:
    using Vec3T = Vector3<T>;
    Vec3T center;
    T radius;

    Sphere(const Vec3T &center, T radius) : center(center), radius(radius) {}
    Sphere() {}

    bool IsColliding(const Vec3T &particle, Vec3T &result) const override
    {
        Vec3T dx = particle - center;
        T dx_norm = dx.norm();
        if (dx_norm < radius)
        {
            result = center + radius * dx / dx_norm;
            return true;
        }
        else
        {
            return false;
        }
    }
};

// TODO: using concept introduced in c++20
template <typename T>
class SimBase
{
protected:
    using Vec3T = Vector3<T>;
    using Vec3I = Vector3<int>;

    std::vector<Vec3T> x;
    std::vector<Vec3T> x_reset;
    std::vector<Vec3T> v;
    std::vector<Vec3T> f;

    // egdes and rest_length
    std::vector<int> e;
    std::vector<T> l;

    unsigned int n_particles;
    unsigned int n_edges;
    unsigned int fixed_node_0;
    unsigned int fixed_node_1;
    Vec3T fixed_node_v;
    Sphere<T> sphere;

public:
    SimBase(unsigned int n_particles, unsigned int n_edges, const std::vector<Vec3T> &pos, const std::vector<int> &edges);
    virtual ~SimBase();

    // void SetPosition(const TV &pos);
    void Step(T dt, int max_steps);
    virtual void Substep(T dt){};
    virtual void Reset() = 0;

    void SetX(unsigned index, const Vec3T &pos);
    Vec3T GetX(unsigned index) const;
    void AddFixedNodes(unsigned int node_0, unsigned int node_1);
    void SetFixedNodeVelocity(Vec3T velocity) { fixed_node_v = velocity; }
    void AddCollision(const Sphere<T> collision);
    void UpdateCollision(const Vec3T &pos);

    // for debug
    std::vector<std::string> logs;
};

template <typename T>
inline SimBase<T>::SimBase(unsigned int n_particles, unsigned int n_edges, const std::vector<Vec3T> &pos, const std::vector<int> &edges)
    : n_particles(n_particles),
      n_edges(n_edges),
      x(n_particles, Vector3<T>::Zero()),
      f(n_particles, Vector3<T>::Zero()),
      v(n_particles, Vector3<T>::Zero()),
      x_reset(n_particles, Vector3<T>::Zero()),
      e(n_edges * 2), l(n_edges)
{
    std::copy(pos.begin(), pos.end(), x.begin());
    std::copy(pos.begin(), pos.end(), x_reset.begin());
    for (size_t i = 0; i < n_edges; i++)
    {
        unsigned int node_0 = edges[i * 2 + 0];
        unsigned int node_1 = edges[i * 2 + 1];
        const Vec3T &pos_0 = pos[node_0];
        const Vec3T &pos_1 = pos[node_1];
        e[i * 2 + 0] = node_0;
        e[i * 2 + 1] = node_1;

        l[i] = (pos_0 - pos_1).norm();
    }
}

template <typename T>
inline SimBase<T>::~SimBase()
{
    // for (size_t i = 0; i < collisions.size(); i++)
    // {
    //     delete collisions[i];
    // }
}

// template <typename T>
// inline void SimBase<T>::SetPosition(const TV &pos)
// {
//     assert(pos.size() == n_particles);
//     std::copy(pos.begin(), pos.end(), x.begin());
// }

template <typename T>
inline void SimBase<T>::Step(T dt, int max_steps)
{
    for (size_t i = 0; i < max_steps; i++)
    {
        Substep(dt);

        if (x[fixed_node_0][2] > 5.0f || x[fixed_node_0][2] < -5.0f)
        {
            fixed_node_v *= -1;
        }

        x[fixed_node_0] += fixed_node_v * dt;
        x[fixed_node_1] += fixed_node_v * dt;
    }
}

template <typename T>
inline void SimBase<T>::SetX(unsigned index, const Vec3T &pos)
{
    x[index] = pos;
}

template <typename T>
inline Vector3<T> SimBase<T>::GetX(unsigned index) const
{
    return x[index];
}

template <typename T>
inline void SimBase<T>::AddFixedNodes(unsigned int node_0, unsigned int node_1)
{
    fixed_node_0 = node_0;
    fixed_node_1 = node_1;
}

template <typename T>
inline void SimBase<T>::AddCollision(Sphere<T> collision)
{
    sphere = collision;
}

template <typename T>
inline void SimBase<T>::UpdateCollision(const Vec3T &pos)
{
    sphere.center = pos;
}
