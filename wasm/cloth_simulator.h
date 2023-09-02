#pragma once

#include "cloth.h"
#include <stdio.h>
class ClothSim
{
private:
    Cloth<float> m_cloth;
public:
    ClothSim(int x_segments, int y_segments);
    float Step(float dt);
    float GetPositionX(int index);
    float GetPositionY(int index);
    float GetPositionZ(int index);
    void SetPosition(int index, float x, float y, float z);
    static int test01;
};