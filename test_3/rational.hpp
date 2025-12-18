#pragma once

//////////

#include <ostream>

//////////

class Rational
{
public:
    Rational(int n = 0, int d = 1);

    Rational& operator+=(const Rational& r);
    Rational& operator-=(const Rational& r);
    Rational& operator*=(const Rational& r);
    Rational& operator/=(const Rational& r);

    int num() const
    {
        return n_;
    }

    int den() const
    {
        return d_;
    }

private:
    int n_;
    int d_;
};

//////////

Rational operator+(Rational a, const Rational& b);
Rational operator-(Rational a, const Rational& b);
Rational operator*(Rational a, const Rational& b);
Rational operator/(Rational a, const Rational& b);

//////////

std::ostream& operator<<(std::ostream& os, const Rational& r);
