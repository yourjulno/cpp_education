// main.cpp

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>

class Stream
{
public:
    using token_t = std::variant<char, double>;

    explicit Stream(const std::string& text)
        : m_stream(text + ';')
    {
    }

    bool empty()
    {
        return m_stream.peek() == ';';
    }

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
                return token_t(x);

            default:
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

    void put(const token_t& token)
    {
        m_token = token;
        m_has_token = true;
    }

private:
    std::stringstream m_stream;
    token_t m_token = ';';
    bool m_has_token = false;
};

class Calculator
{
public:
    double evaluate(const std::string& text) const
    {
        Stream stream(text);

        if (stream.empty())
        {
            return 0.0;
        }

        const double result = expression(stream);

        const auto token = stream.get();
        if (!std::holds_alternative<char>(token) || std::get<char>(token) != ';')
        {
            throw std::runtime_error("unexpected tail");
        }

        return result;
    }

private:
    static void expect(Stream& stream, const char expected)
    {
        const auto token = stream.get();
        if (!std::holds_alternative<char>(token) || std::get<char>(token) != expected)
        {
            throw std::runtime_error("missing closing bracket");
        }
    }

    static double factorial(const double x)
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
        const int n = static_cast<int>(std::round(x));

        for (int i = 1; i <= n; ++i)
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
            const auto token = stream.get();

            if (!std::holds_alternative<char>(token))
            {
                throw std::runtime_error("operator expected");
            }

            switch (std::get<char>(token))
            {
                case '+':
                    x += term(stream);
                    break;

                case '-':
                    x -= term(stream);
                    break;

                default:
                    stream.put(token);
                    return x;
            }
        }
    }

    double term(Stream& stream) const
    {
        double x = power(stream);

        while (true)
        {
            const auto token = stream.get();

            if (!std::holds_alternative<char>(token))
            {
                throw std::runtime_error("operator expected");
            }

            switch (std::get<char>(token))
            {
                case '*':
                    x *= power(stream);
                    break;

                case '/':
                {
                    const double y = power(stream);
                    if (std::abs(y) <= 1e-12)
                    {
                        throw std::runtime_error("division by zero");
                    }
                    x /= y;
                    break;
                }

                case '%':
                {
                    const double y = power(stream);
                    if (std::abs(y) <= 1e-12)
                    {
                        throw std::runtime_error("modulo by zero");
                    }
                    x = std::fmod(x, y);
                    break;
                }

                default:
                    stream.put(token);
                    return x;
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
        const auto token = stream.get();

        if (std::holds_alternative<char>(token))
        {
            switch (std::get<char>(token))
            {
                case '+':
                    return prefix(stream);

                case '-':
                    return -prefix(stream);

                default:
                    stream.put(token);
                    break;
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
            const auto token = stream.get();

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
        const auto token = stream.get();

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
                const double x = expression(stream);
                expect(stream, ')');
                return x;
            }

            case '[':
            {
                const double x = expression(stream);
                expect(stream, ']');
                return x;
            }

            case '{':
            {
                const double x = expression(stream);
                expect(stream, '}');
                return x;
            }

            default:
                throw std::runtime_error("primary expected");
        }
    }
};

struct TestCase
{
    std::string expression;
    double expected = 0.0;
};

struct TestSummary
{
    std::size_t total = 0;
    std::size_t passed = 0;
    std::size_t failed = 0;
};

std::string trim(const std::string& text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {
        return "";
    }

    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool nearly_equal(const double lhs, const double rhs, const double epsilon = 1e-9)
{
    return std::abs(lhs - rhs) <= epsilon;
}

TestCase parse_test_case(const std::string& line)
{
    const auto pos = line.rfind('=');
    if (pos == std::string::npos)
    {
        throw std::runtime_error("missing '=' separator");
    }

    const std::string expression = trim(line.substr(0, pos));
    const std::string expected_text = trim(line.substr(pos + 1));

    if (expression.empty())
    {
        throw std::runtime_error("empty expression");
    }

    if (expected_text.empty())
    {
        throw std::runtime_error("empty expected value");
    }

    std::stringstream stream(expected_text);
    double expected = 0.0;
    stream >> expected;

    if (!stream)
    {
        throw std::runtime_error("invalid expected value");
    }

    stream >> std::ws;
    if (!stream.eof())
    {
        throw std::runtime_error("unexpected tail in expected value");
    }

    return TestCase{expression, expected};
}

TestSummary run_tests_from_file(const std::string& path, const Calculator& calculator)
{
    std::ifstream input(path, std::ios::in);
    if (!input)
    {
        throw std::runtime_error("cannot open input file: " + path);
    }

    TestSummary summary;
    std::string line;
    std::size_t line_number = 0;

    while (std::getline(input, line))
    {
        ++line_number;

        const std::string cleaned = trim(line);
        if (cleaned.empty() || cleaned[0] == '#')
        {
            continue;
        }

        ++summary.total;

        try
        {
            const TestCase test_case = parse_test_case(cleaned);
            const double actual = calculator.evaluate(test_case.expression);

            if (nearly_equal(actual, test_case.expected))
            {
                ++summary.passed;
                std::cout << "[OK]   line " << line_number
                          << ": " << test_case.expression
                          << " = " << actual << '\n';
            }
            else
            {
                ++summary.failed;
                std::cout << "[FAIL] line " << line_number
                          << ": " << test_case.expression
                          << ", expected " << test_case.expected
                          << ", got " << actual << '\n';
            }
        }
        catch (const std::exception& exception)
        {
            ++summary.failed;
            std::cout << "[FAIL] line " << line_number
                      << ": " << exception.what() << '\n';
        }
    }

    return summary;
}

int main(int argc, char* argv[])
{
    try
    {
        const std::string path = (argc > 1) ? argv[1] : "tests.txt";

        const Calculator calculator;
        const TestSummary summary = run_tests_from_file(path, calculator);

        std::cout << '\n'
                  << "total  : " << summary.total << '\n'
                  << "passed : " << summary.passed << '\n'
                  << "failed : " << summary.failed << '\n';

        return (summary.failed == 0) ? 0 : 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "error: " << exception.what() << '\n';
        return 1;
    }
}

/*
tests.txt

# expression = expected_result
2 + 3 * 4 = 14
(2 + 3) * 4 = 20
[2 + 3] * {4 + 1} = 25
10 % 3 = 1
17 % 5 = 2
2 ^ 3 = 8
2 ^ 3 ^ 2 = 512
5! = 120
3!! = 720
2 * 3! = 12
{2 + 3}! = 120
*/