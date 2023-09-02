#pragma once

#include <Eigen/Dense>
#include <vector>

template<class T, int rows, int cols>
using Matrix = Eigen::Matrix<T, rows, cols>;

template<class T, int rows>
using Vector = Eigen::Matrix<T, rows, 1>;

template<class T>
using Vector3 = Vector<T, 3>;

template<typename T>
class Cloth
{
public:
    std::vector<Vector3<T>> x;
    std::vector<std::pair<unsigned int, unsigned int>> springs;
    unsigned int x_segments;
    unsigned int y_segments;
    unsigned int n_nodes;
public:
    Cloth(unsigned int x_segments, unsigned int y_segments);
    //void AssignPosition(unsigned int index, Vector3<T> position);
};

template <typename T>
Cloth<T>::Cloth(unsigned int x_segments, unsigned int y_segments)
    : x_segments(x_segments),
      y_segments(y_segments),
      n_nodes((x_segments + 1) * (y_segments + 1)),
      x((x_segments + 1) * (y_segments + 1))
{
    assert(x_segments > 0 && y_segments > 0);
    springs.reserve(3 * x_segments * y_segments + x_segments + y_segments);

    for (unsigned int i = 0; i < x_segments; ++i)
    {
        for (unsigned int j = 0; j < y_segments; ++j)
        {
            /*
                2:i*(y+1)+j+1
                |\
                | \
                |  \
                |___\ 3:(i+1)*(y+1)+j
            1:i*(y+1)+j
            */

            unsigned int p1 = i * (y_segments + 1) + j;
            unsigned int p2 = i * (y_segments + 1) + j + 1;
            unsigned int p3 = (i + 1) * (y_segments + 1) + j;

            springs.emplace_back(p1, p2);
            springs.emplace_back(p2, p3);
            springs.emplace_back(p3, p1);
        }
    }
}


//template<typename T>
//void Cloth<T>::AssignPosition(unsigned int index, Vector3<T> position)
//{
//    assert(index < n_nodes);
//    x[index] = position;
//}

