#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& name) : m_type(type), m_name(name)
{
}

BlockInfo::~BlockInfo()
{
}

BlockType BlockInfo::GetType() const
{
    return m_type;
}

void BlockInfo::SetDurability(int durability)
{
    m_durability = durability;
}

int BlockInfo::GetDurability() const
{
    return m_durability;
}

void BlockInfo::SetWHUV(float w, float h, float u, float v){
    m_height = h;
    m_width = w;
    m_U = u;
    m_V = v;
}

float BlockInfo::GetW() const{
    return m_width;
}
float BlockInfo::GetH() const{
    return m_height;
}
float BlockInfo::GetU() const{
    return m_U;
}
float BlockInfo::GetV() const{
    return m_V;
}

void BlockInfo::Show() const
{
    std::cout << "Type: " << m_type << std::endl;
    std::cout << "Nom: " << m_name << std::endl;
    std::cout << "Durabilite: " << m_durability << std::endl;
}


