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

void Chunk::Update(BlockInfo* blockInfo[])
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
                    if (bt != BTYPE_AIR)
                    {
                        AddBlockToMesh(vd, count, bt, x, y, z, blockInfo[bt]->GetU(), blockInfo[bt]->GetV(), blockInfo[bt]->GetW(), blockInfo[bt]->GetH());
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
void Chunk::AddBlockToMesh(VertexBuffer::VertexData *vd, int &count, BlockType bt, int x, int y, int z, float u, float v, float w, float h)
{
    int xInitial = x;
    int zInitial = z;

    x = x + m_PositionX;
    z = z + m_PosistionY;

    if (GetBlock(xInitial, y + 1, zInitial) == BTYPE_AIR)
    {
        //top
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u, v+h);
    }
    if (xInitial == CHUNK_SIZE_X || xInitial == 0)
    {
        //Right
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .8f, .8f, .8f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .8f, .8f, .8f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, u, v+h);
        //left
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .8f, .8f, .8f, u, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .8f, .8f, .8f, u, v+h);
    }
    if (zInitial == CHUNK_SIZE_Y || zInitial == 0)
    {

        //Back
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .7f, .7f, .7f, u, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .7f, .7f, .7f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .7f, .7f, .7f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .7f, .7f, .7f, u, v+h);
        //front
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v+h);
        
    }
    
    if (y > 0 && GetBlock(xInitial, y - 1, zInitial) == BTYPE_AIR)
    {
        //bot
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .7f, .7f, .7f, u, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .7f, .7f, .7f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .7f, .7f, .7f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .7f, .7f, .7f, u, v+h);
    }
    
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
