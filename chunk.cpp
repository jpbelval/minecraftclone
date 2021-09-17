#include <iostream>
#include "chunk.h"

Chunk::Chunk() : BlockArray3d::BlockArray3d(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{
    std::cout << "ctor Chunk" << std::endl;
}
Chunk::~Chunk(){
    std::cout << "dtor Chunk" << std::endl;
}

void Chunk::RemoveBlock(int x, int y, int z)
{
    m_blocks[x + ( z * m_x ) + ( y * m_z * m_x ) ] = BlockType::BTYPE_AIR;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type){
    m_blocks[x + ( z * m_x ) + ( y * m_z * m_x ) ] = type;
}

BlockType Chunk::GetBlock(int x, int y, int z) const{
    return m_blocks[x + ( z * m_x ) + ( y * m_z * m_x ) ];
}


