#include "playerdata.h"

PlayerData::PlayerData(int id) : m_id(id)
{

}

PlayerData::~PlayerData()
{
    
}

int PlayerData::GetId()
{
    return m_id;
}

Vector3f PlayerData::GetPosition()
{
    return m_Position;
}

void PlayerData::SetId(int id)
{
    m_id = id;
}

void PlayerData::SetPosition(Vector3f position)
{
    m_Position = position;
}