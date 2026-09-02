#ifndef MATRIXND_HPP
#define MATRIXND_HPP

#include "iterator_traits.hpp"
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <new>

template <typename T>
class MatrixND
{
private:
    std::vector<size_t> dims_;
    size_t total_elements_;
    void** layers_; // Raiz da arvore de ponteiros
    T* data_;       // Ponteiro para o inicio dos dados contiguos

public:
    // Construtor
    MatrixND (const std::vector<size_t>& dims) : dims_ (dims), total_elements_ (1), layers_ (nullptr), data_ (nullptr)
    {
        if (dims_.size() < 2)
        {
            throw std::invalid_argument ("MatrixND requer pelo menos 2 dimensoes.");
        }

        size_t num_dims = dims_.size();
        size_t total_pointers = 0;
        size_t current_layer_ptrs = 1;

        // 1. Calcula ponteiros intermediarios e total de elementos
        for (size_t i = 0; i < num_dims - 1; ++i)
        {
            current_layer_ptrs *= dims_[i];
            total_pointers += current_layer_ptrs;
        }
        total_elements_ = current_layer_ptrs * dims_[num_dims - 1];

        // 2. Tamanhos e alinhamento de memoria
        size_t pointers_bytes = total_pointers * sizeof (void*);
        size_t alignment = alignof (T);
        size_t padding = (alignment - (pointers_bytes % alignment)) % alignment;
        size_t data_offset = pointers_bytes + padding;
        size_t data_bytes = total_elements_ * sizeof (T);

        // 3. Alocacao unica
        unsigned char* raw = static_cast<unsigned char*> (std::malloc (data_offset + data_bytes));
        if (!raw)
        {
            throw std::bad_alloc();
        }

        layers_ = reinterpret_cast<void**> (raw);
        data_ = reinterpret_cast<T*> (raw + data_offset);

        // 4. Encadeamento dos ponteiros das camadas
        void** current_layer = layers_;
        void** next_layer = layers_ + dims_[0];
        size_t count = 1;

        for (size_t d = 0; d < num_dims - 1; ++d)
        {
            count *= dims_[d];

            if (d == num_dims - 2)
            {
                // Ultima camada de ponteiros -> aponta para T*
                for (size_t i = 0; i < count; ++i)
                {
                    current_layer[i] = data_ + (i * dims_[d + 1]);
                }
            }
            else
            {
                // Camada intermediaria de ponteiros -> aponta para void**
                size_t step = dims_[d + 1];
                for (size_t i = 0; i < count; ++i)
                {
                    current_layer[i] = next_layer + (i * step);
                }
                current_layer = next_layer;
                next_layer += count * step;
            }
        }
    }

    // Destrutor
    ~MatrixND()
    {
        std::free (layers_);
    }

    // RAII: Desabilita copia para evitar double-free do ponteiro bruto
    MatrixND (const MatrixND&) = delete;
    MatrixND& operator= (const MatrixND&) = delete;

    // Move Semantics
    MatrixND (MatrixND&& other) noexcept
        : dims_ (std::move (other.dims_)),
          total_elements_ (other.total_elements_),
          layers_ (other.layers_),
          data_ (other.data_)
    {
        other.layers_ = nullptr;
        other.data_ = nullptr;
        other.total_elements_ = 0;
    }

    MatrixND& operator= (MatrixND&& other) noexcept
    {
        if (this != &other)
        {
            std::free (layers_);
            dims_ = std::move (other.dims_);
            total_elements_ = other.total_elements_;
            layers_ = other.layers_;
            data_ = other.data_;

            other.layers_ = nullptr;
            other.data_ = nullptr;
            other.total_elements_ = 0;
        }
        return *this;
    }

    // --- ITERADORES CONTÍGUOS (Compatíveis com iterator_traits.hpp e STL) ---
    T* begin()
    {
        return data_;
    }
    T* end()
    {
        return data_ + total_elements_;
    }
    const T* begin() const
    {
        return data_;
    }
    const T* end() const
    {
        return data_ + total_elements_;
    }

    // --- ACESSO MULTI-NÍVEL DE PONTEIROS ---
    // Retorna a raiz da arvore de ponteiros convertida para o tipo desejado (ex: T***)
    template <typename PointerType>
    PointerType get_root()
    {
        return reinterpret_cast<PointerType> (layers_);
    }

    template <typename PointerType>
    PointerType get_root() const
    {
        return reinterpret_cast<PointerType> (layers_);
    }

    // Indexador da primeira dimensao
    void* operator[] (size_t index)
    {
        return layers_[index];
    }
    const void* operator[] (size_t index) const
    {
        return layers_[index];
    }

    // Metodos utilitarios
    size_t num_dimensions() const
    {
        return dims_.size();
    }
    size_t dim (size_t index) const
    {
        return dims_.at (index);
    }
    size_t total_elements() const
    {
        return total_elements_;
    }
};

#endif
