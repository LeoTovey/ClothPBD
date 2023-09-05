#pragma once

#include <Eigen/Dense>
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
    using V = Vector3<T>;
    V center;
    T radius;

    Sphere(const V &center, T radius) : center(center), radius(radius) {}
    Sphere(){}

    bool IsColliding(const V &particle, V &result) const override
    {
        V dx = particle - center;
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
    using TV = std::vector<Vector3<T>>;
    using V = Vector3<T>;

    TV x;
    TV v;
    TV f;

    unsigned int n_particles;
    unsigned int fixed_node_0;
    unsigned int fixed_node_1;
    V fixed_node_v;
    Sphere<T> sphere;

public:
    SimBase(unsigned int n_particles);
    virtual ~SimBase();

    // void SetPosition(const TV &pos);
    void Step(T dt, int max_steps);
    virtual void Substep(T dt){};
    virtual void Reset() = 0;

    void SetX(unsigned index, const V& pos);
    V GetX(unsigned index) const;
    void AddFixedNodes(unsigned int node_0, unsigned int node_1);
    void SetFixedNodeVelocity(V velocity){fixed_node_v = velocity;}
    void AddCollision(const Sphere<T> collision);
    void UpdateCollision(const V&pos);
    
    // for debug
    std::vector<std::string> logs;
};

template <typename T>
inline SimBase<T>::SimBase(unsigned int n_particles)
    : n_particles(n_particles),
      x(n_particles, Vector3<T>::Zero()),
      f(n_particles, Vector3<T>::Zero()),
      v(n_particles, Vector3<T>::Zero())
{
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

        if (x[fixed_node_0][2] > 4.0f || x[fixed_node_0][2] < -4.0f)
        {
            fixed_node_v *= -1;
        }

        x[fixed_node_0] += fixed_node_v * dt;
        x[fixed_node_1] += fixed_node_v * dt;
    }
}

template <typename T>
inline void SimBase<T>::SetX(unsigned index, const V &pos)
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
inline void SimBase<T>::UpdateCollision(const V &pos)
{
    sphere.center = pos;
}
