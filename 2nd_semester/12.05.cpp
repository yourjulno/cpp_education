#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
public:

    using token_t = std::variant<char, double>;

//  ------------------------------------------------------------------------------------

    Stream(std::string const& string)
        : m_stream(string + ';')
    {
    }

//  ------------------------------------------------------------------------------------

    bool empty()
    {
        return m_stream.peek() == ';';
    }

//  ------------------------------------------------------------------------------------

    token_t get()
    {
        if (m_has_token)
        {
            m_has_token = false;
            return m_token;
        }

        char x = '\0';
        m_stream >> x;

        if (!m_stream)
        {
            return token_t(';');
        }

        switch (x)
        {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
            case '!':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case ';':
            {
                return token_t(x);
            }

            default:
            {
                if (std::isdigit(static_cast<unsigned char>(x)) || x == '.')
                {
                    m_stream.unget();

                    double y = 0.0;
                    m_stream >> y;

                    return token_t(y);
                }

                throw std::runtime_error("unexpected token");
            }
        }
    }

//  ------------------------------------------------------------------------------------

    void put(token_t const& token)
    {
        m_token = token;
        m_has_token = true;
    }

private:

    std::stringstream m_stream;

    token_t m_token = ';';

    bool m_has_token = false;
};

////////////////////////////////////////////////////////////////////////////////////////

class Calculator
{
public:

    double evaluate(std::string const& string) const
    {
        Stream stream(string);

        if (stream.empty())
        {
            return 0.0;
        }

        const double result = expression(stream);

        if (const auto token = stream.get(); !std::holds_alternative<char>(token) || std::get<char>(token) != ';')
        {
            throw std::runtime_error("unexpected tail");
        }

        return result;
    }

//  ------------------------------------------------------------------------------------

    void test() const
    {
        std::string string;

        std::cout << "enter expression(s), ';' to stop\n";

        while (std::getline(std::cin >> std::ws, string))
        {
            if (string == ";")
            {
                break;
            }

            try
            {
                std::cout << string << " = " << evaluate(string) << '\n';
            }
            catch (std::exception const& exception)
            {
                std::cout << "error : " << exception.what() << '\n';
            }
        }
    }

private:

    static void expect(Stream& stream, char expected)
    {
        if (auto token = stream.get(); !std::holds_alternative<char>(token) || std::get<char>(token) != expected)
        {
            throw std::runtime_error("missing closing bracket");
        }
    }

//  ------------------------------------------------------------------------------------

    static double factorial(double x)
    {
        if (x < 0.0)
        {
            throw std::runtime_error("factorial of negative number");
        }

        if (std::abs(x - std::round(x)) > 1e-9)
        {
            throw std::runtime_error("factorial is defined only for integers");
        }

        double result = 1.0;

        for (int i = 1; i <= static_cast<int>(std::round(x)); ++i)
        {
            result *= i;
        }

        return result;
    }

    double expression(Stream& stream) const
    {
        double x = term(stream);

        while (true)
        {
            auto token = stream.get();

            if (!std::holds_alternative<char>(token))
            {
                throw std::runtime_error("operator expected");
            }

            switch (std::get<char>(token))
            {
                case '+':
                {
                    x += term(stream);
                    break;
                }

                case '-':
                {
                    x -= term(stream);
                    break;
                }

                default:
                {
                    stream.put(token);
                    return x;
                }
            }
        }
    }

    double term(Stream& stream) const
    {
        double x = power(stream);

        while (true)
        {
            auto token = stream.get();

            if (!std::holds_alternative<char>(token))
            {
                throw std::runtime_error("operator expected");
            }

            switch (std::get<char>(token))
            {
                case '*':
                {
                    x *= power(stream);
                    break;
                }

                case '/':
                {
                    x /= power(stream);
                    break;
                }

                case '%':
                {
                    x = std::fmod(x, power(stream));
                    break;
                }

                default:
                {
                    stream.put(token);
                    return x;
                }
            }
        }
    }

    double power(Stream& stream) const
    {
        const double x = prefix(stream);

        const auto token = stream.get();

        if (std::holds_alternative<char>(token) && std::get<char>(token) == '^')
        {
            return std::pow(x, power(stream));
        }

        stream.put(token);
        return x;
    }

    double prefix(Stream& stream) const
    {
        if (auto token = stream.get(); std::holds_alternative<char>(token))
        {
            switch (std::get<char>(token))
            {
                case '+':
                {
                    return prefix(stream);
                }

                case '-':
                {
                    return -prefix(stream);
                }

                default:
                {
                    stream.put(token);
                    break;
                }
            }
        }
        else
        {
            stream.put(token);
        }

        return postfix(stream);
    }

    double postfix(Stream& stream) const
    {
        double x = primary(stream);

        while (true)
        {
            auto token = stream.get();

            if (std::holds_alternative<char>(token) && std::get<char>(token) == '!')
            {
                x = factorial(x);
            }
            else
            {
                stream.put(token);
                return x;
            }
        }
    }

    double primary(Stream& stream) const
    {
        auto token = stream.get();

        if (std::holds_alternative<double>(token))
        {
            return std::get<double>(token);
        }

        if (!std::holds_alternative<char>(token))
        {
            throw std::runtime_error("primary expected");
        }

        switch (std::get<char>(token))
        {
            case '(':
            {
                double x = expression(stream);
                expect(stream, ')');
                return x;
            }

            case '[':
            {
                double x = expression(stream);
                expect(stream, ']');
                return x;
            }

            case '{':
            {
                double x = expression(stream);
                expect(stream, '}');
                return x;
            }

            default:
            {
                throw std::runtime_error("primary expected");
            }
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Calculator calculator;

//  ------------------------------------------------------------------------------------

    assert(calculator.evaluate("2 + 3 * 4") == 14.0);
    assert(calculator.evaluate("(2 + 3) * 4") == 20.0);
    assert(calculator.evaluate("[2 + 3] * {4 + 1}") == 25.0);

    assert(calculator.evaluate("10 % 3") == 1.0);
    assert(calculator.evaluate("17 % 5") == 2.0);

    assert(calculator.evaluate("2 ^ 3") == 8.0);
    assert(calculator.evaluate("2 ^ 3 ^ 2") == 512.0);

    assert(calculator.evaluate("5!") == 120.0);
    assert(calculator.evaluate("3!!") == 720.0);
    assert(calculator.evaluate("2 * 3!") == 12.0);
    assert(calculator.evaluate("{2 + 3}!") == 120.0);

//  ------------------------------------------------------------------------------------

    calculator.test();
}

////////////////////////////////////////////////////////////////////////////////////////