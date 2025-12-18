#include <exception>
#include <string>
#include <test_3/rational.hpp>

class Exception : public std::exception
{
public:
    explicit Exception(const std::string& message)
        : message_(message)
    {}

    const char* what() const noexcept override
    {
        return message_.c_str();
    }

private:
    std::string message_;
};


Rational::Rational(int n, int d)
    : n_(n), d_(d)
{
    if (d == 0)
    {
        throw Exception("Rational: zero denominator");
    }
}

#include <iostream>
#include <vector>
#include <variant>
#include <optional>
#include <stdexcept>

int main()
{
    try
    {
        Rational r(1, 0);
    }
    catch (const std::exception& e)
    {
        std::cerr << "std::exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Unknown exception\n";
    }

    try
    {
        std::vector<int> v;
        v.reserve(static_cast<size_t>(-1));
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "bad_alloc\n";
    }

    try
    {
        std::variant<int, double> v = 1;
        std::get<double>(v);
    }
    catch (const std::bad_variant_access& e)
    {
        std::cerr << "bad_variant_access\n";
    }

    try
    {
        std::optional<int> o;
        o.value();
    }
    catch (const std::bad_optional_access& e)
    {
        std::cerr << "bad_optional_access\n";
    }

    try
    {
        std::vector<int> v;
        v.resize(v.max_size() + 1);
    }
    catch (const std::length_error& e)
    {
        std::cerr << "length_error\n";
    }

    try
    {
        std::vector<int> v(3);
        v.at(10);
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "out_of_range\n";
    }

    return 0;
}

