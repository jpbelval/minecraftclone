#include "chunk.h"
#include <climits>
#include <iostream>

Chunk::Chunk(int x, int y) : m_PositionX(x), m_PosistionY(y), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_isDirty(true)
{
    m_blocks.Reset(BTYPE_AIR);
    for (int x = 0; x < CHUNK_SIZE_X; ++x)
    {
        for (int z = 0; z < CHUNK_SIZE_Z; ++z)
        {
            for (int y = 0; y < CHUNK_SIZE_Y; ++y)
            {
                if (y > 1)
                {
                    SetBlock(x, y, z, BTYPE_AIR);
                }
                else{
                    SetBlock(x, y, z, BTYPE_DIRT);
                }
            }
        }
    }
}

Chunk::~Chunk()
{
}

void Chunk::RemoveBlock(int x, int y, int z)
{
    m_blocks.Set(x, y, z, BTYPE_AIR);
}

void Chunk::SetBlock(int x, int y, int z, BlockType type)
{
    m_blocks.Set(x, y, z, type);
}

BlockType Chunk::GetBlock(int x, int y, int z)
{
    return m_blocks.Get(x, y, z);
}

void Chunk::Update()
{
    if (m_isDirty)
    {
        int maxVertexCount = (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z) * (6 * 4);
        VertexBuffer::VertexData *vd = new VertexBuffer::VertexData[maxVertexCount];
        int count = 0;
        for (int x = 0; x < CHUNK_SIZE_X; ++x)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z)
            {
                for (int y = 0; y < 16; ++y)
                {
                    if (count > USHRT_MAX)
                        break;
                    BlockType bt = GetBlock(x, y, z);
                    // && GetBlock(x,y+1,z) == BTYPE_AIR
                    if (bt != BTYPE_AIR)
                    {
                        AddBlockToMesh(vd, count, bt, x, y, z);
                    }
                }
            }
        }
        if (count > USHRT_MAX)
        {
            count = USHRT_MAX;
            std::cout << "[ Chunk:: Update] Chunk data truncaned , too much vertices to have a 16bit index" << std::endl;
        }
        m_vertexBuffer.SetMeshData(vd, count);
        delete[] vd;
    }
    m_isDirty = false;
}
void Chunk::AddBlockToMesh(VertexBuffer::VertexData *vd, int &count, BlockType bt, int x, int y, int z)
{
    x = x + m_PositionX;
    z = z + m_PosistionY;
    // front
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 0.f, 1.f);
    //Right
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .8f, .8f, .8f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .8f, .8f, .8f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, 0.f, 1.f);
    //Back
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .7f, .7f, .7f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .7f, .7f, .7f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .7f, .7f, .7f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .7f, .7f, .7f, 0.f, 1.f);
    //left
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .8f, .8f, .8f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .8f, .8f, .8f, 0.f, 1.f);
    //top
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, 0.f, 1.f);
    //bot
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .7f, .7f, .7f, 0.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .7f, .7f, .7f, 1.f, 0.f);
    vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .7f, .7f, .7f, 1.f, 1.f);
    vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .7f, .7f, .7f, 0.f, 1.f);
}

void Chunk::Render() const
{
    m_vertexBuffer.Render();
}

bool Chunk::IsDirty() const
{
    return m_isDirty;
}

// Update mesh
