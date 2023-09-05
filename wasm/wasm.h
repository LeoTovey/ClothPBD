#pragma once

#include "mass_spings.h"
#include "implicit_solver.h"
#include <memory.h>

class Foo
{
public:
    int getVal()
    {
        return 1;
    }
    void setVal(int v)
    {

    }
};
class WASMSim
{
private:
    SimBase<float> *sim = nullptr;
    using Vector3f = Vector3<float>;
    int n_particles;
    int n_edges;

public:
    WASMSim(int n_particles);
    ~WASMSim();

    void InitPosition(int index, float x, float y, float z);
    void InitSim(int sim_type, float width, float height, int x_segments, int y_segments, float k, float mass, float damping, float gravity);

    void SetFixedNodeVelocity(float v0, float v1, float v2);
    void AddSphere(float r);
    void UpdateSphere(float x, float y, float z);

    std::vector<Vector3f> pos_temp;

    void Step(float dt, int max_steps);
    float GetPositionX(int index);
    float GetPositionY(int index);
    float GetPositionZ(int index);
    const char *Print();
};
