#ifndef BLOCKARRAY3D_H__
#define BLOCKARRAY3D_H__

#include "define.h"

template<class T>
class Array3d
{
    public:
        Array3d(int x, int y, int z);
        ~Array3d();
        Array3d(const Array3d& array);

        void Set(int x, int y, int z, T type);
        T Get(int x, int y, int z) const;

        void Reset(T type);

    private:
        int To1dIndex(int x, int y, int z) const;

    private:
        int m_x, m_y, m_z;
        T* m_blocks;
};

#endif // BLOCKARRAY3D_H__
