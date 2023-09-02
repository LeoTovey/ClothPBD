
#include "cloth_simulator.h"

int ClothSim::test01 = 0;
ClothSim::ClothSim(int x_segments, int y_segments) : m_cloth(x_segments, y_segments)
{

} 

float ClothSim::Step(float dt)
{
    // std::for_each(m_cloth.x.begin(), m_cloth.x.end(), [](Eigen::Vector3f& val){
    //     val[0] += 0.01;
    // });

    // for (auto& node : m_cloth.x)
    // {
    //     node(0) = node(0) + 0.1;
    //     printf("%f", node(0));
    // }

    
    for (size_t i = 0; i < m_cloth.x.size(); i++)
    {
        m_cloth.x[i][1] = m_cloth.x[i][1] + 0.01;
    }
    
    printf("123");
    test01 += 1;
    return m_cloth.x.size();
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
void ClothSim::SetPosition(int index, float x, float y, float z)
{
    m_cloth.x[index] = {x, y, z};
}
