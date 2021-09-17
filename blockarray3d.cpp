#include <iostream>
#include "blockarray3d.h"


BlockType BlockArray3d::Get(int x, int y, int z) const{
    return m_blocks [ x + ( z * m_x ) + ( y * m_z * m_x ) ];
}

void BlockArray3d::Set(int x, int y, int z, BlockType type){
    m_blocks[ x + ( z * m_x ) + ( y * m_z * m_x ) ] = type;
}

void BlockArray3d::Reset(BlockType type){
    for (int i = 0; i < m_x * m_y * m_z; i++)
        m_blocks[i] = type;
    
}

BlockArray3d::BlockArray3d(int x, int y, int z){
    m_x = x;
    m_y = y;
    m_z = z;
    m_blocks = new BlockType[m_x * m_y * m_z];
}

BlockArray3d::~BlockArray3d(){
    delete[] m_blocks;
}
