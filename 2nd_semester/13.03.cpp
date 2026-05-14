#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

bool is_space_only(std::string const& string)
{
    return std::ranges::all_of(string
                               ,
                               [](unsigned char c) { return std::isspace(c) != 0; });
}

void transform(std::string const& path_1, std::string const& path_2)
{
    auto string = (std::stringstream() << std::fstream(path_1, std::ios::in).rdbuf()).str();

    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator)
    {
        if (*iterator == '\'')
        {
            do
            {
                ++iterator;
            }
            while (iterator != std::end(string) &&
                   !(*iterator == '\'' && *std::prev(iterator) != '\\'));

            continue;
        }

        if (*iterator == '"')
        {
            do
            {
                ++iterator;
            }
            while (iterator != std::end(string) &&
                   !(*iterator == '"' && *std::prev(iterator) != '\\'));

            continue;
        }

        if (*iterator == 'R' && std::next(iterator) != std::end(string) && *std::next(iterator) == '"')
        {
            auto delimiter_begin = std::next(iterator, 2);
            auto open = delimiter_begin;

            while (open != std::end(string) && *open != '(')
            {
                ++open;
            }

            std::string delimiter(delimiter_begin, open);

            auto end = open;
            while (end != std::end(string))
            {
                if (*end == ')')
                {
                    auto probe = std::next(end);
                    auto match = true;

                    for (char c : delimiter)
                    {
                        if (probe == std::end(string) || *probe != c)
                        {
                            match = false;
                            break;
                        }
                        ++probe;
                    }

                    if (match && probe != std::end(string) && *probe == '"')
                    {
                        iterator = probe;
                        break;
                    }
                }

                ++end;
            }

            continue;
        }

        if (*iterator == '/')
        {
            if (std::next(iterator) != std::end(string) && *std::next(iterator) == '/')
            {
                auto end = std::next(iterator, 2);

                while (end != std::end(string) && *end != '\n')
                {
                    ++end;
                }

                iterator = string.erase(iterator, end);

                if (iterator == std::end(string))
                {
                    break;
                }

                --iterator;
            }
            else if (std::next(iterator) != std::end(string) && *std::next(iterator) == '*')
            {
                auto end = std::next(iterator, 2);

                while (std::next(end) != std::end(string) &&
                       !(*end == '*' && *std::next(end) == '/'))
                {
                    ++end;
                }

                if (std::next(end) != std::end(string))
                {
                    iterator = string.erase(iterator, std::next(end, 2));

                    if (iterator == std::end(string))
                    {
                        break;
                    }

                    --iterator;
                }
            }
        }
    }

    std::stringstream result;
    std::stringstream input(string);
    std::string line;

    while (std::getline(input, line))
    {
        if (!line.empty() && !is_space_only(line))
        {
            result << line << '\n';
        }
    }

    std::fstream(path_2, std::ios::out) << result.str();
}

int main()
{
    auto path_1 = "source.cpp";
    auto path_2 = "output.cpp";

    transform(path_1, path_2);

    std::cout << "main : enter char : ";
    std::cin.get();

    std::filesystem::remove(path_2);
}