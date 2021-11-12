#ifndef CHUNK_H__
#define CHUNK_H__
#include "array3d.h"
#include "vertexbuffer.h"
#include "blockinfo.h"

class Chunk
{
    public:
    Chunk();
    ~Chunk();

    void RemoveBlock(int x, int y, int z);
    void SetBlock(int x, int y, int z, BlockType type);
    BlockType GetBlock(int x, int y, int z);
    void Update(BlockInfo* blockInfo[]);
    void Render() const;
    bool IsDirty() const;
    void AddBlockToMesh(VertexBuffer::VertexData *vd, int &count, BlockType bt, int x, int y, int z, float u, float v, float w, float h);

    private:
    Array3d<BlockType> m_blocks;
    VertexBuffer m_vertexBuffer;
    bool m_isDirty;
};

#endif // CHUNK_H__
