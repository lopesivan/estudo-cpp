#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include "iterator_traits.hpp"
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

    // Desabilita cópia para evitar double-free acidental com ponteiro bruto
    Matrix2D (const Matrix2D&) = delete;
    Matrix2D& operator= (const Matrix2D&) = delete;

    // Suporte a Move Semantics (opcional, para eficiência)
    Matrix2D (Matrix2D&& other) noexcept
        : rows_ (other.rows_), cols_ (other.cols_), grid_ (other.grid_)
    {
        other.grid_ = nullptr;
        other.rows_ = 0;
        other.cols_ = 0;
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

    // --- MÉTODOS DE MANIPULAÇÃO CORRIGIDOS ---

    void swap_rows (size_t row_a, size_t row_b)
    {
        auto it_a = begin();
        auto it_b = begin();

        custom::advance (it_a, row_a * cols_);
        custom::advance (it_b, row_b * cols_);

        for (size_t c = 0; c < cols_; ++c)
        {
            custom::iter_swap (it_a, it_b);
            ++it_a;
            ++it_b;
        }
    }

    void swap_cols (size_t col_a, size_t col_b)
    {
        auto it_a = begin();
        auto it_b = begin();

        custom::advance (it_a, col_a);
        custom::advance (it_b, col_b);

        for (size_t r = 0; r < rows_; ++r)
        {
            custom::iter_swap (it_a, it_b);

            if (r < rows_ - 1)
            {
                custom::advance (it_a, cols_);
                custom::advance (it_b, cols_);
            }
        }
    }

    void swap_rows_fast (size_t row_a, size_t row_b)
    {
        if (row_a >= rows_ || row_b >= rows_) return;

        // Troca direta dos ponteiros que apontam para as linhas
        T* temp = grid_[row_a];
        grid_[row_a] = grid_[row_b];
        grid_[row_b] = temp;
    }
};

#endif
