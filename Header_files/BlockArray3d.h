#ifndef BLOCKARRAY3D_H__
#define BLOCKARRAY3D_H__

class BlockArray3d {
    protected:
        int* m_blocks;
    public:
        BlockArray3d(int x, int y, int z){
            m_blocks = new int[x * y * z];
        };

        ~BlockArray3d(){
            delete m_blocks;
        };

        BlockArray3d(const BlockArray3d& blockArrayOrigin);

        void Set(int x, int y, int z, BlockType type);
        BlockType Get(int x, int y, int z) const;

        void Reset(BlockType type);
};

#endif