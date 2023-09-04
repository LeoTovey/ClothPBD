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

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "Node 0: " << node_0 << ", Node 1: " << node_1 << ", Rest Length: " << rest_length;
        return oss.str();
    }
};

template <typename T>
class Cloth
{
public:
    std::vector<Vector3<T>> x;
    std::vector<Spring<T>> springs;
    unsigned int x_segments;
    unsigned int y_segments;
    T width;
    T height;
    unsigned int n_nodes;

public:
    Cloth(T width, T height, unsigned int x_segments, unsigned int y_segments);
    // void AssignPosition(unsigned int index, Vector3<T> position);
};

template <typename T>
Cloth<T>::Cloth(T width, T height, unsigned int x_segments, unsigned int y_segments)
    : x_segments(x_segments),
      y_segments(y_segments),
      width(width),
      height(height),
      n_nodes((x_segments + 1) * (y_segments + 1)),
      x((x_segments + 1) * (y_segments + 1))
{
    assert(x_segments > 0 && y_segments > 0);
    springs.reserve(3 * x_segments * y_segments + x_segments + y_segments);

    T spring_0 = width / (T)x_segments;
    T spring_1 = height / (T)y_segments;
    T spring_2 = std::sqrt(spring_0 * spring_0 + spring_1 * spring_1);

    for (unsigned int i = 0; i < x_segments; ++i)
    {
        for (unsigned int j = 0; j < y_segments; ++j)
        {
            unsigned int p1 = j * (x_segments + 1) + i;
            unsigned int p2 = (j + 1) * (x_segments + 1) + i;
            unsigned int p3 = j * (x_segments + 1) + i + 1;

            springs.emplace_back(p1, p2, spring_1);
            springs.emplace_back(p2, p3, spring_2);
            springs.emplace_back(p3, p1, spring_0);
        }
    }

    for (size_t i = n_nodes - x_segments - 1; i < n_nodes - 1; i++)
    {
        springs.emplace_back(i, i + 1, spring_0);
    }

    for (size_t i = x_segments; i < n_nodes - 1; i += (x_segments + 1))
    {
        springs.emplace_back(i, i + x_segments + 1, spring_1);
    }
}

// template<typename T>
// void Cloth<T>::AssignPosition(unsigned int index, Vector3<T> position)
//{
//     assert(index < n_nodes);
//     x[index] = position;
// }

