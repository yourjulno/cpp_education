#include <format>
#include <initializer_list>
#include <algorithm>
#include <utility>
#include <iostream>

template<typename T>
class Vector
{
public:
    Vector() : m_array(nullptr), m_size(0), m_capacity(0)
    {
        std::cout << std::format("Vector:: Vector (1)\n");
    }

//  --------------------------------------------------------------------------------

    Vector(std::initializer_list<T> list) :
        m_size(std::size(list)),
        m_capacity(std::size(list))
    {
        std::cout << "Vector:: Vector (2)\n";

        m_array = m_capacity ? new T[m_capacity]{} : nullptr;

        std::ranges::copy(list, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector const& other) :
        m_size(other.m_size),
        m_capacity(other.m_capacity)
    {
        std::cout << "Vector:: Vector (3)\n";

        m_array = m_capacity ? new T[m_capacity]{} : nullptr;

        std::ranges::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

//  --------------------------------------------------------------------------------

    Vector(Vector&& other) :
        m_array(std::exchange(other.m_array, nullptr)),
        m_size(std::exchange(other.m_size, 0)),
        m_capacity(std::exchange(other.m_capacity, 0))
    {
        std::cout << "Vector:: Vector (4)\n";
    }

//  --------------------------------------------------------------------------------

    ~Vector()
    {
        std::cout << "Vector::~Vector\n";

        delete[] m_array;
    }

//  --------------------------------------------------------------------------------

    auto& operator=(Vector other)
    {
        std::cout << "Vector::operator= (4)\n";

        swap(other);

        return *this;
    }

//  --------------------------------------------------------------------------------

    void swap(Vector& other)
    {
        std::swap(m_array, other.m_array);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
    }

//  --------------------------------------------------------------------------------

    std::size_t capacity() const
    {
        return m_capacity;
    }

    std::size_t size() const
    {
        return m_size;
    }

    void push_back(T value)
    {
        if (m_size >= m_capacity)
        {
            std::size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;

            T* new_array = new T[new_capacity]{};

            if (m_array)
            {
                std::ranges::copy(m_array, m_array + m_size, new_array);
                delete[] m_array;
            }

            m_array = new_array;
            m_capacity = new_capacity;
        }


        m_array[m_size] = value;
        ++m_size;
    }

    void clear()
    {
        m_size = 0;
    }

    bool empty() const
    {
        return m_size == 0;
    }

private:
    T* m_array = nullptr;
    std::size_t m_size = 0;
    std::size_t m_capacity = 0;
};

////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs)
{
    lhs.swap(rhs);
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Vector<double> vector_1;

    Vector vector_2 = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    Vector vector_3 = vector_2;

    Vector vector_4 = std::move(vector_3);

//  --------------------------------------

    vector_3 = vector_2;

    vector_4 = std::move(vector_3);

//  --------------------------------------

    swap(vector_1, vector_2);


    Vector<int> v;
    std::cout << std::format("Initial - Size: {}, Capacity: {}, Empty: {}\n",
               v.size(), v.capacity(), v.empty());

    v.push_back(10);
    std::cout << std::format("After push_back(10) - Size: {}, Capacity: {}, Empty: {}\n",
               v.size(), v.capacity(), v.empty());

    v.push_back(20);
    std::cout << std::format("After push_back(20) - Size: {}, Capacity: {}, Empty: {}\n",
               v.size(), v.capacity(), v.empty());

    v.push_back(30);
    std::cout << std::format("After push_back(30) - Size: {}, Capacity: {}, Empty: {}\n",
               v.size(), v.capacity(), v.empty());

    v.clear();
    std::cout << std::format("After clear() - Size: {}, Capacity: {}, Empty: {}\n",
               v.size(), v.capacity(), v.empty());
}