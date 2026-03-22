#include <boost/numeric/ublas/matrix.hpp>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace fib {

    using u64 = unsigned long long;
    using Matrix = boost::numeric::ublas::matrix<u64>;

    Matrix identity_2x2()
    {
        Matrix I(2, 2);
        I(0, 0) = 1; I(0, 1) = 0;
        I(1, 0) = 0; I(1, 1) = 1;
        return I;
    }

    Matrix multiply_2x2(const Matrix& a, const Matrix& b)
    {
        Matrix c(2, 2);
        c(0, 0) = a(0, 0) * b(0, 0) + a(0, 1) * b(1, 0);
        c(0, 1) = a(0, 0) * b(0, 1) + a(0, 1) * b(1, 1);
        c(1, 0) = a(1, 0) * b(0, 0) + a(1, 1) * b(1, 0);
        c(1, 1) = a(1, 0) * b(0, 1) + a(1, 1) * b(1, 1);
        return c;
    }

    Matrix pow_2x2(Matrix base, u64 exp)
    {
        Matrix result = identity_2x2();

        while (exp > 0)
        {
            if (exp & 1ULL)
                result = multiply_2x2(result, base);

            base = multiply_2x2(base, base);
            exp >>= 1ULL;
        }

        return result;
    }

    u64 fibonacci(const u64 n)
    {
        if (n > 93ULL)
            throw std::overflow_error("F(n) overflows unsigned long long for n > 93");

        if (n == 0ULL) return 0ULL;

        Matrix Q(2, 2);
        Q(0, 0) = 1; Q(0, 1) = 1;
        Q(1, 0) = 1; Q(1, 1) = 0;

        const Matrix Qn = pow_2x2(Q, n);
        return Qn(0, 1); // F(n)
    }

} // namespace fib

int main()
{
    using fib::fibonacci;

    assert(fibonacci(0) == 0);
    assert(fibonacci(1) == 1);
    assert(fibonacci(2) == 1);
    assert(fibonacci(3) == 2);
    assert(fibonacci(10) == 55);
    assert(fibonacci(93) == 12200160415121876738ULL);

    std::cout << "F(10) = " << fibonacci(10) << "\n";
}