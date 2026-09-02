#include <iostream>

template <class T> struct Vec3
{
    T x, y, z;

    // Soma componente a componente: (x1+x2, y1+y2, z1+z2)
    Vec3 operator+(const Vec3& o) const
    {
        return {x + o.x, y + o.y, z + o.z};
    }

    // Subtração componente a componente: (x1-x2, y1-y2, z1-z2)
    Vec3 operator-(const Vec3& o) const
    {
        return {x - o.x, y - o.y, z - o.z};
    }

    // Escala o vetor por um escalar s: (x*s, y*s, z*s)
    // Cobre v * escalar (lado direito). Para escalar * v ver o
    // operator* livre logo abaixo da struct.
    Vec3 operator*(T s) const
    {
        return {x * s, y * s, z * s};
    }

    // Produto escalar (dot product): retorna um número T, não um
    // Vec3. dot = x1*x2 + y1*y2 + z1*z2
    T dot(const Vec3& o) const
    {
        return x * o.x + y * o.y + z * o.z;
    }

    // Produto vetorial (cross product): retorna outro Vec3,
    // perpendicular aos dois vetores originais (definido só em 3D).
    Vec3 cross(const Vec3& o) const
    {
        return {y * o.z - z * o.y,
                z * o.x - x * o.z,
                x * o.y - y * o.x};
    }

    // Multiplicação componente a componente (produto de Hadamard):
    // (x1*x2, y1*y2, z1*z2). NÃO é dot nem cross — resultado é
    // outro Vec3, sem significado geométrico direto (ex: blending
    // de RGB).
    Vec3 operator*(const Vec3& o) const
    {
        return {x * o.x, y * o.y, z * o.z};
    }
};

// Permite escalar * Vec3 (ex: 3.0f * a), delegando pro operator*(T)
// membro, que só cobre Vec3 * escalar (ex: a * 3.0f).
template <class T> Vec3<T> operator*(T s, const Vec3<T>& v)
{
    return v * s;
}

// Imprime como (x, y, z)
template <class T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
{
    return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

int main()
{
    Vec3<float> a{1.0f, 2.0f, 3.0f};
    Vec3<float> b{4.0f, 5.0f, 6.0f};
    Vec3<float> c{5.0f, 9.0f, -20.0f};
    Vec3<float> d{5.0f, 2.0f, 18.0f};

    // Avaliado esquerda->direita: ((((a+b)*c)*a)-b)
    // *c e *a aqui são Hadamard (elemento a elemento), não
    // dot/cross.
    std::cout << "a             = " << a << '\n';
    std::cout << "(a + b)*c*a-b = " << (a + b) * c * a - b << '\n';
    std::cout << "a*a*a         = " << a * a * a << '\n';
    std::cout << "3*a           = " << 3.0f * a << '\n';
    std::cout << "3*a + 3*a     = " << 3.0f * a + 3.0f * a << '\n';

    return 0;
}
