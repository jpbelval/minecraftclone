#include "playerdata.h"

PlayerData::PlayerData(int id) : m_id(id), m_Position(Vector3f(0,0,0))
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