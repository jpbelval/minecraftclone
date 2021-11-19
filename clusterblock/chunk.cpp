#include "chunk.h"
#include <climits>
#include <iostream>

Chunk::Chunk(int x, int y) : m_PositionX(x), m_PositionY(y), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_isDirty(true)
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
                    SetBlock(x, y, z, BTYPE_GRASS);
                }
            }
        }
    }
    GenererStructure();

}

void Chunk::GenererStructure(){
    //Arche
    SetBlock(9,2,2, BTYPE_COBBLE);
    SetBlock(9,3,2, BTYPE_COBBLE);
    SetBlock(9,4,2, BTYPE_COBBLE);
    SetBlock(9,4,3, BTYPE_COBBLE);
    SetBlock(9,4,4, BTYPE_COBBLE);
    SetBlock(9,3,4, BTYPE_COBBLE);
    SetBlock(9,2,4, BTYPE_COBBLE);

    //Escalier
    SetBlock(5,2,7, BTYPE_COBBLE);
    SetBlock(6,3,7, BTYPE_COBBLE);
    SetBlock(7,4,7, BTYPE_COBBLE);
    SetBlock(8,5,7, BTYPE_COBBLE);

    //Mur X = le meme
    SetBlock(6,2,11, BTYPE_COBBLE);
    SetBlock(6,2,12, BTYPE_COBBLE);
    SetBlock(6,2,13, BTYPE_COBBLE);
    SetBlock(6,2,14, BTYPE_COBBLE);
    SetBlock(6,3,14, BTYPE_COBBLE);
    SetBlock(6,3,11, BTYPE_COBBLE);
    SetBlock(6,3,12, BTYPE_COBBLE);
    SetBlock(6,3,13, BTYPE_COBBLE);
    SetBlock(6,3,14, BTYPE_COBBLE);
    SetBlock(6,4,11, BTYPE_COBBLE);
    SetBlock(6,4,12, BTYPE_COBBLE);
    SetBlock(6,4,13, BTYPE_COBBLE);
    SetBlock(6,4,14, BTYPE_COBBLE);

    //Mur Z = le meme
    SetBlock(11,2,14, BTYPE_PLANK);
    SetBlock(12,2,14, BTYPE_PLANK);
    SetBlock(13,2,14, BTYPE_PLANK);
    SetBlock(14,2,14, BTYPE_PLANK);
    SetBlock(14,3,14, BTYPE_PLANK);
    SetBlock(11,3,14, BTYPE_PLANK);
    SetBlock(12,3,14, BTYPE_PLANK);
    SetBlock(13,3,14, BTYPE_PLANK);
    SetBlock(14,3,14, BTYPE_PLANK);
    SetBlock(11,4,14, BTYPE_PLANK);
    SetBlock(12,4,14, BTYPE_PLANK);
    SetBlock(13,4,14, BTYPE_PLANK);
    SetBlock(14,4,14, BTYPE_PLANK);
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
    int yInitial = y;
    int zInitial = z;

    x = x + m_PositionX;
    z = z + m_PositionY;

    if (GetBlock(xInitial, y + 1, zInitial) == BTYPE_AIR)
    {
        //top
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, 1.f, 1.f, 1.f, u, v+h);
    }
    if (x >= 0 && (xInitial == CHUNK_SIZE_X || xInitial == 0 || GetBlock(x + 1, yInitial, zInitial) == BTYPE_AIR || GetBlock(x - 1, yInitial, zInitial) == BTYPE_AIR))
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
    if (z >= 0 && (zInitial == CHUNK_SIZE_Z || zInitial == 0 || GetBlock(xInitial, yInitial, z + 1) == BTYPE_AIR || GetBlock(xInitial, yInitial, z - 1) == BTYPE_AIR))
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
