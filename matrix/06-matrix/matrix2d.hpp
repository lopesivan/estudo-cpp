#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <cstdlib>

template <typename T>
class Matrix2D
{
private:
    size_t rows_;
    size_t cols_;
    T** grid_;

public:
    Matrix2D (size_t rows, size_t cols) : rows_ (rows), cols_ (cols)
    {
        size_t pointer_bytes = rows * sizeof (T*);
        size_t data_bytes = rows * cols * sizeof (T);

        unsigned char* raw = (unsigned char*)std::malloc (pointer_bytes + data_bytes);
        grid_ = (T**)raw;

        T* data_ptr = (T*) (raw + pointer_bytes);
        for (size_t i = 0; i < rows; ++i)
        {
            grid_[i] = data_ptr + i * cols;
        }
    }

    ~Matrix2D()
    {
        std::free (grid_);
    }

    T* operator[] (size_t row)
    {
        return grid_[row];
    }
    const T* operator[] (size_t row) const
    {
        return grid_[row];
    }

    T* begin()
    {
        return grid_[0];
    }
    T* end()
    {
        return grid_[0] + (rows_ * cols_);
    }
    const T* begin() const
    {
        return grid_[0];
    }
    const T* end() const
    {
        return grid_[0] + (rows_ * cols_);
    }

    size_t rows() const
    {
        return rows_;
    }
    size_t cols() const
    {
        return cols_;
    }
};

#endif
