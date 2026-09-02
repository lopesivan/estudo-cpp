// #include "iterator_traits.hpp"
#include <iostream>
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
        std::free (grid_); // Liberação única
    }

    // Acesso estilo C: matrix[r][c]
    T* operator[] (size_t row)
    {
        return grid_[row];
    }
    const T* operator[] (size_t row) const
    {
        return grid_[row];
    }

    // Iteradores C++ para percorrer a matriz linearmente
    T* begin()
    {
        return grid_[0];
    }
    T* end()
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

int main()
{
    Matrix2D<double> mat (5, 6);

    // Loop duplo tradicional por coordenadas
    for (size_t r = 0; r < mat.rows(); ++r)
    {
        for (size_t c = 0; c < mat.cols(); ++c)
        {
            mat[r][c] = (r + 1) * 10.0 + c; // Atribui valor baseado em linha/coluna
        }
    }


    double* ptr = mat.begin();
    size_t total_elements = mat.rows() * mat.cols();

    for (size_t i = 0; i < total_elements; ++i)
    {
        printf ("%.1f ", *ptr++);

        // Quebra a linha ao atingir o fim de cada coluna
        if ((i + 1) % mat.cols() == 0)
        {
            printf ("\n");
        }
    }


}
