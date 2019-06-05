//------------------------------------------------------------------------------
//  Generic 2-dimensional array class ---- Matrix
//
//  2016/05/22, Copyright (c) 2016 MIKAMI, Naoki
//------------------------------------------------------------------------------

#ifndef MIKAMI_MATRIX_HPP
#define MIKAMI_MATRIX_HPP

#include "Array.hpp"

namespace Mikami
{
    template <class T> class Matrix
    {
    public:
        explicit Matrix(int rows = 1, int cols = 1);
        Matrix(int rows, int cols, T initialValue);
        void Fill(T val);
        void SetSize(int rows, int cols);                           // setting size
        Array<T>& operator[](int i) { return vv_[i]; }              // assign by element
        const Array<T>& operator[](int i) const { return vv_[i]; }  //get element
        int Rows() const { return vv_.Length(); }
        int Cols() const { return vv_[0].Length(); }
    private:
        Array< Array<T> > vv_;
    };

//-----------------------------------------------------------------------
// implementation of generic 2-dimensional array class
//-----------------------------------------------------------------------
    template <class T> Matrix<T>::Matrix(int rows, int cols): vv_(rows)
    {
        Array<T> a(cols);
        for (int i=0; i<rows; i++) vv_[i] = a;
    }

    template <class T> Matrix<T>::Matrix(int rows, int cols, T initialValue)
            : vv_(rows)
    {
        Array<T> a(cols, initialValue);
        for (int i=0; i<rows; i++) vv_[i] = a;
    }

    template <class T> void Matrix<T>::Fill(T val)
    {
        for (int n=0; n<Rows(); n++) vv_[n].Fill(val);
    }

    template <class T> void Matrix<T>::SetSize(int rows, int cols)
    {
        Array<T> a(cols);
        vv_.SetSize(rows);
        for (int i=0; i<rows; i++) vv_[i] = a;
    }
}
#endif  // MIKAMI_MATRIX_HPP

