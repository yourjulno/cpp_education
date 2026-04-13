#include <cmath>
#include <iostream>
#include <optional>
#include <utility>
#include <variant>

using Roots = std::variant<
    double,
    std::pair<double, double>,
    std::monostate
>;

using Result = std::optional<Roots>;

constexpr double epsilon = 1e-9;

Result solve(double a, double b, double c)
{
    if (std::abs(a) < epsilon)
    {
        if (std::abs(b) < epsilon)
        {
            if (std::abs(c) < epsilon)
            {
                return Roots{std::monostate{}};
            }
            else
            {
                return std::nullopt;
            }
        }
        else
        {
            return Roots{-c / b};
        }
    }

    if (double d = b * b - 4 * a * c; d < -epsilon)
    {
        return std::nullopt;
    }
    else if (std::abs(d) < epsilon)
    {
        return Roots{-b / (2 * a)};
    }
    else
    {
        const double sqrt_d = std::sqrt(d);
        double x1 = (-b - sqrt_d) / (2 * a);
        double x2 = (-b + sqrt_d) / (2 * a);
        return Roots{std::make_pair(x1, x2)};
    }
}

class Visitor
{
public:
    void operator()(double x) const
    {
        std::cout << x;
    }

    void operator()(const std::pair<double, double>& p) const
    {
        std::cout << p.first << " " << p.second;
    }

    void operator()(std::monostate) const
    {
    }
};

int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;

    if (Result result = solve(a, b, c))
    {
        std::visit(Visitor{}, *result);
    }

    return 0;
}