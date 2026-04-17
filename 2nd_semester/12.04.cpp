#include <algorithm>
#include <cassert>
#include <regex>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////

using namespace std::literals;

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto text = R"(Contacts:
ivan.petrov@example.com
support@test.org
Write to admin@my-domain.net or sales@company.ru for details.
)"s;

//  --------------------------------------------------------------------------------

    std::regex pattern
    (
        R"(([a-zA-Z0-9._%+-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))"
    );

//  --------------------------------------------------------------------------------

    std::vector < std::string > emails;

    std::vector < std::string > domains;

//  --------------------------------------------------------------------------------

    auto begin = std::sregex_iterator(std::cbegin(text), std::cend(text), pattern);

    auto end = std::sregex_iterator();

//  --------------------------------------------------------------------------------

    auto lambda = [&emails, &domains](std::smatch const & match)
    {
        emails .push_back(match[0].str()); // весь email
        domains.push_back(match[2].str()); // домен из группы
    };

//  --------------------------------------------------------------------------------

    std::for_each(begin, end, lambda);

//  --------------------------------------------------------------------------------

    assert
    (
        emails == std::vector < std::string >
        ({
            "ivan.petrov@example.com",
            "support@test.org",
            "admin@my-domain.net",
            "sales@company.ru"
        })
    );

//  --------------------------------------------------------------------------------

    assert
    (
        domains == std::vector < std::string >
        ({
            "example.com",
            "test.org",
            "my-domain.net",
            "company.ru"
        })
    );
}

////////////////////////////////////////////////////////////////////////////////////