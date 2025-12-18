module rational;

//////////

namespace math
{
    Rational::Rational(int n, int d)
        : n_(n)
        , d_(d)
    {
    }

    Rational& Rational::operator+=(const Rational& r)
    {
        n_ = n_ * r.d_ + r.n_ * d_;
        d_ = d_ * r.d_;
        return *this;
    }

    Rational& Rational::operator-=(const Rational& r)
    {
        n_ = n_ * r.d_ - r.n_ * d_;
        d_ = d_ * r.d_;
        return *this;
    }

    Rational& Rational::operator*=(const Rational& r)
    {
        n_ *= r.n_;
        d_ *= r.d_;
        return *this;
    }

    Rational& Rational::operator/=(const Rational& r)
    {
        n_ *= r.d_;
        d_ *= r.n_;
        return *this;
    }

    int Rational::num() const
    {
        return n_;
    }

    int Rational::den() const
    {
        return d_;
    }

    Rational operator+(Rational a, const Rational& b)
    {
        return a += b;
    }

    Rational operator-(Rational a, const Rational& b)
    {
        return a -= b;
    }

    Rational operator*(Rational a, const Rational& b)
    {
        return a *= b;
    }

    Rational operator/(Rational a, const Rational& b)
    {
        return a /= b;
    }

    std::ostream& operator<<(std::ostream& os, const Rational& r)
    {
        return os << r.num() << "/" << r.den();
    }
}
