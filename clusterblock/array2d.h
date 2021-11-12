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
        int GetCol() const;
        int GetRow() const;

    private:
        int To1dIndex(int x, int y) const;

    private:
        int m_x, m_y;
        T* m_data;
};

template<class T>
Array2d<T>::Array2d(int x, int y) : m_x(x), m_y(y)
{
    m_data = new T[m_x * m_y];
}

template<class T>
Array2d<T>::~Array2d()
{
    delete [] m_data;
}

template<class T>
Array2d<T>::Array2d(const Array2d& array) : m_x(array.m_x), m_y(array.m_y)
{
    m_data = new T[m_x * m_y];
    for(int i = 0; i < m_x * m_y; ++i)
        m_data[i] = array.m_data[i];
}
template<class T>
void Array2d<T>::Set(int x, int y, T type)
{
    m_data[To1dIndex(x, y)] = type;
}

template<class T>
T Array2d<T>::Get(int x, int y) const
{
    return m_data[To1dIndex(x, y)];
}

template<class T>
int Array2d<T>::To1dIndex(int x, int y) const
{
    return x + (y * m_x);
}

template<class T>
int Array2d<T>::GetCol() const {
    return m_y;
}

template<class T>
int Array2d<T>::GetRow() const {
    return m_x;
}
#endif