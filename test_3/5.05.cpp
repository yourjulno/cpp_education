#include <iostream>

//////////

template < typename T >
struct addable
{
    friend T operator+(const T& a, const T& b)
    {
        T r(a);
        r += b;
        return r;
    }
};

template < typename T >
struct subtractable
{
    friend T operator-(const T& a, const T& b)
    {
        T r(a);
        r -= b;
        return r;
    }
};

template < typename T >
struct multipliable
{
    friend T operator*(const T& a, const T& b)
    {
        T r(a);
        r *= b;
        return r;
    }
};

template < typename T >
struct dividable
{
    friend T operator/(const T& a, const T& b)
    {
        T r(a);
        r /= b;
        return r;
    }
};

template < typename T >
struct incrementable
{
    friend T operator++(T& a)
    {
        a += T(1);
        return a;
    }

    friend T operator++(T& a, int)
    {
        T tmp(a);
        ++a;
        return tmp;
    }
};

template < typename T >
struct decrementable
{
    friend T operator--(T& a)
    {
        a -= T(1);
        return a;
    }

    friend T operator--(T& a, int)
    {
        T tmp(a);
        --a;
        return tmp;
    }
};

//////////

class Rational
    : public addable<Rational>
    , public subtractable<Rational>
    , public multipliable<Rational>
    , public dividable<Rational>
    , public incrementable<Rational>
    , public decrementable<Rational>
{
public:
    Rational(int n = 0, int d = 1)
        : n_(n)
        , d_(d)
    {
    }

    Rational& operator+=(const Rational& r)
    {
        n_ = n_ * r.d_ + r.n_ * d_;
        d_ = d_ * r.d_;
        return *this;
    }

    Rational& operator-=(const Rational& r)
    {
        n_ = n_ * r.d_ - r.n_ * d_;
        d_ = d_ * r.d_;
        return *this;
    }

    Rational& operator*=(const Rational& r)
    {
        n_ *= r.n_;
        d_ *= r.d_;
        return *this;
    }

    Rational& operator/=(const Rational& r)
    {
        n_ *= r.d_;
        d_ *= r.n_;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Rational& r)
    {
        return os << r.n_ << "/" << r.d_;
    }

private:
    int n_;
    int d_;
};

//////////

int main()
{
    Rational a(1, 2);
    Rational b(1, 3);

    auto c = a + b;
    auto d = a * b;
    auto e = ++a;

    std::cout << c << "\n";
    std::cout << d << "\n";
    std::cout << e << "\n";
}
