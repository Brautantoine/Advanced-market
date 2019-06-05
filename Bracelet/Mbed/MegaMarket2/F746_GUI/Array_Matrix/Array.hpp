//-----------------------------------------------------------------------
//  Generic Array class
//      If you define "#define DEBUG_ARRAY_CHECK",
//      range check of index is available.
//
//  2016/08/15, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------------------

#include "mbed.h"
#include <new>          // for new, delete, and set_new_handler()

#ifndef MIKAMI_ARRAY_HPP
#define MIKAMI_ARRAY_HPP

namespace Mikami
{
    template <class T> class Array
    {
    public:
        explicit Array(int i = 1) { ArrayNew(i); }  // default constructor
        Array(const Array<T>& a) { Copy(a); }       // copy constructor
        inline Array(int i, T initialValue);        // constructor with initialization
        inline Array(int i, const T val[]);         // constructor with assignment built-in array
        ~Array() { delete[] v_; }                   // destructor
        inline void Fill(T val);                    // fill with same value
        inline void Assign(const T val[]);          // assign built-in array
        void SetSize(int i);                        // setting size
        int Length() const { return size_; }        // get size of array
        Array<T>& operator=(const Array<T>& a);     // assignment
        inline T& operator[](int i);                // assignment by element
        inline const T& operator[](int n) const;    // get element
        operator const T* () const { return v_; }   // type conversion
        operator T* () const { return v_; }         // type conversion

    private:
        T    *v_;
        int  size_;                                 // size of array

        void Range(int pos) const;                  // range checking for Array
        void Copy(const Array<T>& v_src);           // copy of object
        inline void ArrayNew(const int i);          // routine for constructor
        static void MemoryAssignError();            // error message
    };

//-----------------------------------------------------------------------
// implementation of generic array class
//-----------------------------------------------------------------------

    // constructor with initialization
    template <class T> Array<T>::Array(int i, T initialValue)
    {
        ArrayNew(i);
        Fill(initialValue);
    }
                                     
    // constructor with assignment built-in array
    template <class T> Array<T>::Array(int i, const T val[])
    {
        ArrayNew(i);
        Assign(val);
    }

    template <class T> void Array<T>::SetSize(int i)
    {
        delete[] v_;
        v_ = new T[size_ = i];
    }

    // fill with same value
    template <class T> void Array<T>::Fill(T val)
    {
        for (int n=0; n<size_; n++) v_[n] = val;        
    }

    // assign built-in array
    template <class T> void Array<T>::Assign(const T val[])
    {
        for (int n=0; n<size_; n++) v_[n] = val[n];        
    }          

    template <class T> Array<T>& Array<T>::operator=(const Array<T>& a)
    {
        if (this != &a) // prohibition of self-assignment
        {
            delete [] v_;
            Copy(a);
        }
        return *this;
    }

    template <class T> inline T& Array<T>::operator[](int i)
    {
    #ifdef DEBUG_ARRAY_CHECK
        Range(i);       // out of bound ?
    #endif
        return v_[i];
    }

    template <class T> inline const T& Array<T>::operator[](int i) const
    {
    #ifdef DEBUG_ARRAY_CHECK
        Range(i);       // out of bounds ?
    #endif
        return v_[i];
    }

    template <class T> void Array<T>::Range(int pos) const
    {
        if ((pos < 0) || (pos >= size_))
        {
            fprintf(stderr, "\r\nOut of range\r\n");
            while (true);
        }            
    }

    template <class T> void Array<T>::Copy(const Array<T>& v_src)
    {
        v_ = new T[size_ = v_src.size_];
        for (int n=0; n<size_; n++) v_[n] = v_src.v_[n];
    }

    // routine for constructor
    template <class T> void Array<T>::ArrayNew(int i)
    {
        set_new_handler(Array<T>::MemoryAssignError);
        v_ = new T[size_ = i];
    }

    // Message of "Can't allocate to memory!"
    template <class T> void Array<T>::MemoryAssignError()
    {
        fprintf(stderr, "Can't allocate to memory!\r\n");
        while(true);
    }
}
#endif  // MIKAMI_ARRAY_HPP
