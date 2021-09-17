#include <iostream>
#include "blockInfo.h"
#include <string>

BlockInfo::BlockInfo(BlockType type, const std::string& name) : m_type(type), m_nom(name)
{
    std::cout << "ctor BlockInfo" << std::endl;
}
BlockInfo::~BlockInfo()
{
    std::cout << "dtor BLockInfo" << std::endl;
}

void BlockInfo::SetDurability(const int durability)
{
    m_durability = durability;
}
int BlockInfo::GetDurability() const
{
    return m_durability;
}

void BlockInfo::Show() const
{
    std::cout << "nom: " << m_nom << std::endl;
    std::cout << "type: " << m_type << std::endl;
    std::cout << "durabilite: " << GetDurability() << std::endl; 
}
