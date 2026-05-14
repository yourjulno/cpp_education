

#include <cassert>
#include <locale>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

int main()
{
    std::locale locale_1("en_US.UTF-8");

    std::locale locale_2("ru_RU.UTF-8");

    const double ruble_per_usd = 90.0;

    std::string line;
    std::getline(std::cin, line);

    std::stringstream input(line);
    input.imbue(locale_2);

    long double rub_kopecks = 0.0L;

    input >> std::showbase >> std::get_money(rub_kopecks);

    if (input.fail())
    {
        std::cout << "Ошибка ввода\n";
        return 1;
    }

    long double usd_cents = std::round(rub_kopecks / ruble_per_usd);

    std::stringstream output;
    output.imbue(locale_1);

    output << std::showbase << std::put_money(usd_cents);

    std::cout << output.str() << '\n';
}

////////////////////////////////////////////////////////////////////////////////////////////