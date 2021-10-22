#ifndef ARRAY2D_H__
#define ARRAY2D_H__

#include "define.h"

template<class T>
class Array2d
{
    public:
        Array2d(int x, int y);
        ~Array2d();
        Array2d(const Array2d& array);

        void Set(int x, int y, T type);
        T Get(int x, int y) const;

        void Reset(T type);

    private:
        int To1dIndex(int x, int y) const;

    private:
        int m_x, m_y;
        T* m_blocks;
};

#endif