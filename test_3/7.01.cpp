#include <optional>
#include <variant>
#include <utility>

using Roots = std::variant<
    double,
    std::pair<double, double>,
    std::monostate
>;

using Result = std::optional<Roots>;

#include <cmath>

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

    double d = b * b - 4 * a * c;

    if (d < -epsilon)
    {
        return std::nullopt;
    }
    else if (std::abs(d) < epsilon)
    {
        return Roots{-b / (2 * a)};
    }
    else
    {
        double sqrt_d = std::sqrt(d);
        double x1 = (-b - sqrt_d) / (2 * a);
        double x2 = (-b + sqrt_d) / (2 * a);
        return Roots{std::make_pair(x1, x2)};
    }
}

#include <iostream>

int main()
{
    double a, b, c;
    std::cin >> a >> b >> c;

    Result result = solve(a, b, c);

    if (!result)
    {
        return 0;
    }

    const Roots& roots = *result;

    if (std::holds_alternative<double>(roots))
    {
        std::cout << std::get<double>(roots);
    }
    else if (std::holds_alternative<std::pair<double, double>>(roots))
    {
        auto p = std::get<std::pair<double, double>>(roots);
        std::cout << p.first << " " << p.second;
    }
    else
    {
    }

    return 0;
}
