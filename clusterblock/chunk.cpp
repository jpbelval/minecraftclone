#include "chunk.h"
#include <climits>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

Chunk::Chunk(int x, int y) : m_PositionX(x), m_PositionY(y), m_blocks(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), m_isDirty(true), m_isModified(false)
{
    std::string s1 = std::to_string(m_PositionX);
    std::string s2 = std::to_string(m_PositionY);
    std::string s = "Chunk/" + s1 + "," + s2;
    
    std::ifstream Fichier(s, std::fstream::binary);
    if (Fichier.is_open())
    {
        char data[CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z];
        Fichier.read(data, sizeof(data));
        m_blocks.SetData((BlockType*)data);
        std::cout << "1" << std::endl;
    }
    else
    {
        Perlin perlin (16,8,1,95);
        m_blocks.Reset(BTYPE_AIR);
        BlockType bt = NULL;
        for (int x = 0; x < CHUNK_SIZE_X; ++x)
        {
            for (int z = 0; z < CHUNK_SIZE_Z; ++z)
            {   
                float y = perlin.Get((float)(m_PositionX + x) / 2000.f, (float)(m_PositionY + z) / 2000.f) * 25 + 26;
                if (y < 26)
                {
                    bt = BTYPE_PLANK;
                }
                else
                {
                    bt = BTYPE_GRASS;
                }
                SetBlock(x,y,z, bt);
                for (float i = 0; i < y - 1; ++i)
                {
                    SetBlock(x,i,z, BTYPE_COBBLE);
                }
            }
        }  
    }
    

}

Chunk::~Chunk()
{
    if (m_isModified){
        std::cout << "Modo  Chunk deleted" << std::endl;
        EcrireFichier();
    }
}

void Chunk::RemoveBlock(int x, int y, int z)
{
    m_blocks.Set(x, y, z, BTYPE_AIR);
    m_isDirty = true;
    m_isModified = true;
}

void Chunk::SetBlock(int x, int y, int z, BlockType type, bool placeJoueur)
{
    m_blocks.Set(x, y, z, type);
    m_isDirty = true;
    if(placeJoueur)
        m_isModified = true;
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
                for (int y = 0; y < CHUNK_SIZE_Y; ++y)
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
    if (x >= 0 && (xInitial == 0 || xInitial == CHUNK_SIZE_X -1 || GetBlock(xInitial + 1, yInitial, zInitial) == BTYPE_AIR))
    {
        //Right
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, .8f, .8f, .8f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .8f, .8f, .8f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .8f, .8f, .8f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, .8f, .8f, .8f, u, v+h);
    }
    if(x >= 0 && (xInitial == 0 || xInitial == CHUNK_SIZE_X -1 || GetBlock(xInitial - 1, yInitial, zInitial) == BTYPE_AIR)){
        //left
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .8f, .8f, .8f, u, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, .8f, .8f, .8f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, .8f, .8f, .8f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .8f, .8f, .8f, u, v+h);
    }
    if (z >= 0 && (zInitial == 0 || zInitial == CHUNK_SIZE_Z -1 || GetBlock(xInitial, yInitial, zInitial + 1) == BTYPE_AIR))
    {
        
        //front
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z + .5f, 1.f, 1.f, 1.f, u, v+h);
        
    }
    if (z>= 0 && (zInitial == 0 || zInitial == CHUNK_SIZE_Z - 1 || GetBlock(xInitial, yInitial, zInitial - 1) == BTYPE_AIR))
    {
        //Back
        vd[count++] = VertexBuffer::VertexData(x + .5f, y - .5f, z - .5f, .7f, .7f, .7f, u, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y - .5f, z - .5f, .7f, .7f, .7f, u+w, v);
        vd[count++] = VertexBuffer::VertexData(x - .5f, y + .5f, z - .5f, .7f, .7f, .7f, u+w, v+h);
        vd[count++] = VertexBuffer::VertexData(x + .5f, y + .5f, z - .5f, .7f, .7f, .7f, u, v+h);
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

void Chunk::EcrireFichier (){
    std::string s1 = std::to_string(m_PositionX);
    std::string s2 = std::to_string(m_PositionY);
    std::string s = "Chunk/" + s1 + "," + s2;   
    std::ofstream Fichier(s, std::fstream::binary);
    char data[CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z];
    int i = 0;
    for(int x = 0; x < CHUNK_SIZE_X; ++x){
        for(int y = 0; y < CHUNK_SIZE_Y; ++y){
            for(int z = 0; z < CHUNK_SIZE_Z; ++z){
                data[i] = m_blocks.Get(x, y, z);
                i++;
            }
        }
    }   
    Fichier.write((char*)m_blocks.GetPointeur(), CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z);
    //Fichier << std::endl;
    Fichier.close();
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
