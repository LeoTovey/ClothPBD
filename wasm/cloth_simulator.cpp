
#include "cloth_simulator.h"


ClothSim::ClothSim(float width, float height, int x_segments, int y_segments)
    : m_cloth(width, height, x_segments, y_segments)
{
    fixed_node_0 = 0;
    fixed_node_1 = x_segments;
    m_force.resize(m_cloth.n_nodes);
    m_v.resize(m_cloth.n_nodes);
    for (size_t i = 0; i < m_cloth.n_nodes; i++)
    {
        m_force[i] = Vector3f::Zero();
        m_v[i] = Vector3f::Zero();
    }
}

void ClothSim::Step(float delta_time)
{
    int steps = int(delta_time / dt);
    static Vector3f move = Vector3f { 0, 0, 0.00001 };
    for (size_t i = 0; i < steps; i++)
    {
        SubStep(dt);
        Collision();

        if (m_cloth.x[fixed_node_0][2] > 4.0f || m_cloth.x[fixed_node_0][2] < -4.0f)
        {
            move *= -1;
        }

        m_cloth.x[fixed_node_0] += move;
        m_cloth.x[fixed_node_1] += move;
    }

   
    
}

float ClothSim::GetPositionX(int index)
{
    return m_cloth.x[index][0];
}
float ClothSim::GetPositionY(int index)
{
    return m_cloth.x[index][1];
}
float ClothSim::GetPositionZ(int index)
{
    return m_cloth.x[index][2];
}
const char *ClothSim::Print()
{
    // 使用 std::stringstream 来拼接字符串
    std::stringstream ss;
    for (const std::string &log : m_logs)
    {
        ss << log << "\n"; // 用换行符分隔每个字符串
    }
    m_logs.clear();
    std::string result = ss.str();           // 获取拼接后的字符串
    return result.c_str(); // 转换为 const char*
}
void ClothSim::SetPosition(int index, float x, float y, float z)
{
    m_cloth.x[index] = {x, y, z};
}

void ClothSim::UpdateSphere(float x, float y, float z, float r)
{
    sphere_center = {x, y, z};
    sphere_radius = r;
}

void ClothSim::UpdateFixedPoint(int index, float x, float y, float z)
{

}

void ClothSim::SubStep(float dt)
{
   
    for (size_t i = 0; i < m_cloth.n_nodes; i++)
    {
        if (i != fixed_node_0 && i != fixed_node_1)
        {
            m_v[i] += m_force[i] / node_mass;
            m_cloth.x[i] += dt * m_v[i];
        }
    }

    Vector3f g = Vector3f(0.0, -gravity * node_mass, 0.0);
    std::for_each(m_force.begin(), m_force.end(), [g](Vector3f &force)
                  { force = g; });
    for (const auto &spring : m_cloth.springs)
    {
        unsigned int node_0 = spring.node_0;
        unsigned int node_1 = spring.node_1;
        Vector3f p0 = m_cloth.x[node_0];
        Vector3f p1 = m_cloth.x[node_1];

        float current_length = (p0 - p1).norm();
        Vector3f force_dir = (p0 - p1).normalized();

        Vector3f force = -k * (current_length - spring.rest_length) * force_dir;

        m_force[node_0] += force;
        m_force[node_1] -= force;

        // damping
        Vector3f v_ij = m_v[node_0] - m_v[node_1];
        float v_rel = v_ij.dot(force_dir);
        m_force[node_0] -= v_rel * damping * force_dir;
        m_force[node_1] += v_rel * damping * force_dir;
    }
}

void ClothSim::Collision()
{
    for (size_t i = 0; i < m_cloth.n_nodes; i++)
    {
        Vector3f& x = m_cloth.x[i];
        Vector3f dx = x - sphere_center;
        float dx_norm = dx.norm();
        if (dx_norm < sphere_radius)
        {
            m_v[i] += (sphere_center + sphere_radius * dx / dx_norm - x) / dt;
            x = sphere_center + 1.0 * sphere_radius * dx / dx_norm;
        }
    }
}
