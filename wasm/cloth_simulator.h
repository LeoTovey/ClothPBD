#pragma once

#include "cloth.h"
#include <stdio.h>
class ClothSim
{
private:
    using Vector3f = Vector3<float>;
    Cloth<float> m_cloth;
    std::vector<Vector3f> m_force;
    std::vector<Vector3f> m_v;
    unsigned fixed_node_0;
    unsigned fixed_node_1;

    std::vector<std::string> m_logs;

public:
    float k = 1.0f;
    float node_mass = 1.0f;
    float damping = 1.0f;
    float gravity = 0.98;
    float dt = 1e-3;
    Vector3f sphere_center;
    float sphere_radius;
    ClothSim(float width, float height, int x_segments, int y_segments);
    void Step(float dt);
    float GetPositionX(int index);
    float GetPositionY(int index);
    float GetPositionZ(int index);
    const char *Print();
    void SetPosition(int index, float x, float y, float z);
    void UpdateSphere(float x, float y, float z, float r);
    void UpdateFixedPoint(int index, float x, float y, float z);

private:
    void SubStep(float dt);
    void Collision();
};
