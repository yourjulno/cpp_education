#include <limits>

template<int N>
struct Fibonacci
{
    static_assert(N >= 0, "N must be non-negative");
    static_assert(N < std::numeric_limits<int>::max());
    static const int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
    static_assert(Fibonacci<N - 1>::value <= Fibonacci<N>::value,
                  "Integer overflow detected in Fibonacci calculation");
};

template<>
struct Fibonacci<0>
{
    static const int value = 0;
};

template<>
struct Fibonacci<1>
{
    static const int value = 1;
};

template<int N>
constexpr int Fibonacci_v = Fibonacci<N>::value;

static_assert(Fibonacci_v<0> == 0, "Fibonacci(0) should be 0");
static_assert(Fibonacci_v<1> == 1, "Fibonacci(1) should be 1");
static_assert(Fibonacci_v<2> == 1, "Fibonacci(2) should be 1");
static_assert(Fibonacci_v<3> == 2, "Fibonacci(3) should be 2");
static_assert(Fibonacci_v<4> == 3, "Fibonacci(4) should be 3");
static_assert(Fibonacci_v<5> == 5, "Fibonacci(5) should be 5");
static_assert(Fibonacci_v<6> == 8, "Fibonacci(6) should be 8");
static_assert(Fibonacci_v<7> == 13, "Fibonacci(7) should be 13");
static_assert(Fibonacci_v<8> == 21, "Fibonacci(8) should be 21");
static_assert(Fibonacci_v<9> == 34, "Fibonacci(9) should be 34");
static_assert(Fibonacci_v<10> == 55, "Fibonacci(10) should be 55");

int main()
{
    return 0;
}