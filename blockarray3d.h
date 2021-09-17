#ifndef BLOCKARRAY3D_H__
#define BLOCKARRAY3D_H__

#include "define.h"

class BlockArray3d {
    protected:
        BlockType* m_blocks;
        int m_x;
        int m_y;
        int m_z;
    public:
        BlockArray3d(int x, int y, int z);

        ~BlockArray3d();

        BlockArray3d(const BlockArray3d& blockArrayOrigin);

        void Set(int x, int y, int z, BlockType type);
        BlockType Get(int x, int y, int z) const;

        void Reset(BlockType type);
};

#endif