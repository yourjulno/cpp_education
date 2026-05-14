#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <iterator>
#include <print>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

auto make_type(std::filesystem::file_status const& status)
{
    if (std::filesystem::is_directory(status)) { return 'd'; }
    if (std::filesystem::is_regular_file(status)) { return 'f'; }
    if (std::filesystem::is_symlink(status)) { return 'l'; }

    return '?';
}

auto make_permissions(std::filesystem::perms permissions) -> std::string
{
    auto lambda = [permissions](auto x, auto y)
    {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return
    {
        lambda(std::filesystem::perms::owner_read,  'r'),
        lambda(std::filesystem::perms::owner_write, 'w'),
        lambda(std::filesystem::perms::owner_exec,  'x')
    };
}

auto size(std::filesystem::path const& path)
{
    auto total = std::uintmax_t{0};

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (!std::filesystem::is_directory(entry.status()))
            {
                total += std::filesystem::file_size(entry);
            }
        }
    }

    return total;
}

auto size(std::filesystem::directory_entry const& entry)
{
    auto total = std::uintmax_t{0};

    if (std::filesystem::is_regular_file(entry.status()))
    {
        total = std::filesystem::file_size(entry);
    }
    else
    {
        total = ::size(entry.path());
    }

    std::vector<char> units = {'B', 'K', 'M', 'G'};
    auto i = std::size_t{0};

    while (i + 1 < units.size() && total >= (1ULL << 10))
    {
        total /= (1ULL << 10);
        ++i;
    }

    return (std::stringstream() << std::format("{: >4} ({})", total, units[i])).str();
}

void show(std::filesystem::path const& path, std::regex const& expression)
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const& entry : std::filesystem::directory_iterator(path))
        {
            auto const name = entry.path().filename().string();

            if (!std::regex_match(name, expression))
            {
                continue;
            }

            std::print(
                "show : entry : {} | {} | {} | {} | {}\n",
                make_type(entry.status()),
                make_permissions(entry.status().permissions()),
                size(entry),
                std::chrono::floor<std::chrono::seconds>(
                    std::chrono::file_clock::to_sys(entry.last_write_time())
                ),
                name
            );
        }
    }
}

int main(int argc, char* argv[])
{
    auto expression_text = std::string{".*"};
    auto path = std::filesystem::current_path();

    if (argc >= 2)
    {
        expression_text = argv[1];
    }

    if (argc >= 3)
    {
        path = argv[2];
    }

    try
    {
        auto expression = std::regex(expression_text);
        show(path, expression);
    }
    catch (std::regex_error const& exception)
    {
        std::cerr << "regex error: " << exception.what() << '\n';
        return 1;
    }
}