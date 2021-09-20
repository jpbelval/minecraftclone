#include <iostream>
#include "blockarray3d.h"

BlockArray3d::BlockArray3d(int x, int y, int z){
    m_x = x;
    m_y = y;
    m_z = z;
    m_blocks = new BlockType[m_x * m_y * m_z];
    Reset(BTYPE_AIR);
    std::cout << "ctor blockarray3d" << std::endl;
}

BlockArray3d::~BlockArray3d(){
    delete[] m_blocks;
    std::cout << "dtor blockarray3d" << std::endl;
}

BlockArray3d::BlockArray3d(const BlockArray3d& blockArrayOrigin){
    m_x = blockArrayOrigin.m_x;
    m_y = blockArrayOrigin.m_y;
    m_z = blockArrayOrigin.m_z;
    m_blocks = new BlockType[m_x * m_y * m_z];
    for (unsigned int i = 0; i < m_x * m_y * m_z; i++)
        m_blocks[i] = blockArrayOrigin.m_blocks[i];
    
}

BlockType BlockArray3d::Get(int x, int y, int z) const{
    return m_blocks[ x + ( z * m_x ) + ( y * m_z * m_x ) ];
}

void BlockArray3d::Set(int x, int y, int z, BlockType type){
    m_blocks[ x + ( z * m_x ) + ( y * m_z * m_x ) ] = type;
}

void BlockArray3d::Reset(BlockType type){
    for (int i = 0; i < m_x * m_y * m_z; i++)
    {
        m_blocks[i] = type;
    }
    
}

