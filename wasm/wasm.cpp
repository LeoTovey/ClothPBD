#include "wasm.h"

WASMSim::WASMSim(int n_particles) : pos_temp(n_particles)
{
   
}

WASMSim::~WASMSim()
{
    free(sim);
}

void WASMSim::InitPosition(int index, float x, float y, float z)
{
    pos_temp[index] = {x, y, z};
}

void WASMSim::InitSim(int sim_type, float width, float height, int x_segments, int y_segments, float k, float mass, float damping, float gravity)
{
    unsigned int n_nodes = (x_segments + 1) * (y_segments + 1);
    assert(x_segments > 0 && y_segments > 0);
    assert(pos_temp.size() == n_nodes);
    std::vector<int> edges;
    for (unsigned int i = 0; i < x_segments; ++i)
    {
        for (unsigned int j = 0; j < y_segments; ++j)
        {
            unsigned int p1 = j * (x_segments + 1) + i;
            unsigned int p2 = (j + 1) * (x_segments + 1) + i;
            unsigned int p3 = j * (x_segments + 1) + i + 1;
            edges.push_back(p1);
            edges.push_back(p2);

            edges.push_back(p2);
            edges.push_back(p3);

            edges.push_back(p3);
            edges.push_back(p1);
        }
    }

    for (size_t i = n_nodes - x_segments - 1; i < n_nodes - 1; i++)
    {
        edges.push_back(i);
        edges.push_back(i + 1);
    }

    for (size_t i = x_segments; i < n_nodes - 1; i += (x_segments + 1))
    {
        edges.push_back(i);
        edges.push_back(i + x_segments + 1);
    }

    if (sim_type == 0)
    {
        JacobiSimParams<float> params;
        params.k = k;
        params.damping = damping;
        params.gravity = gravity;
        params.mass = mass;
        params.max_iteration = 32;
        sim = new JacobiSim<float>(pos_temp, edges, params);
    }
    else
    {
        MassSpringParams<float> params;
        params.k = k;
        params.damping = damping;
        params.gravity = gravity;
        params.mass = mass;
        sim = new MassSpringSim<float>(pos_temp, edges, params);
    }

    sim->AddFixedNodes(0, x_segments);
}

void WASMSim::SetFixedNodeVelocity(float v0, float v1, float v2)
{
    sim->SetFixedNodeVelocity({v0, v1, v2});
}

void WASMSim::AddSphere(float r)
{
    sim->AddCollision(Sphere<float>({0, 0, 0}, r));
}

void WASMSim::UpdateSphere(float x, float y, float z)
{
    sim->UpdateCollision({x, y, z});
}

void WASMSim::Step(float delta_time, int max_steps)
{
    sim->Step(delta_time, max_steps);
}

float WASMSim::GetPositionX(int index)
{
    Vector3f x = sim->GetX(index);
    return x[0];
}

float WASMSim::GetPositionY(int index)
{
    Vector3f x = sim->GetX(index);
    return x[1];
}

float WASMSim::GetPositionZ(int index)
{
    Vector3f x = sim->GetX(index);
    return x[2];
}

const char *WASMSim::Print()
{
    // 使用 std::stringstream 来拼接字符串
    std::stringstream ss;
    for (const std::string &log : sim->logs)
    {
        ss << log << "\n"; // 用换行符分隔每个字符串
    }
    //sim->logs.clear();
    std::string result = ss.str(); // 获取拼接后的字符串
    return result.c_str();         // 转换为 const char*
}
