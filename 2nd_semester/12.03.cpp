#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::string_view longest_palindrome(std::string_view text)
{
    const std::size_t n = text.size();

    if (n == 0)
    {
        return {};
    }

    std::vector<bool> cache(n * n, false);

    auto index = [n](std::size_t left, std::size_t right)
    {
        return left * n + right;
    };

    std::size_t best_start = 0;
    std::size_t best_length = 1;

    for (std::size_t i = 0; i < n; ++i)
    {
        cache[index(i, i)] = true;
    }

    for (std::size_t length = 2; length <= n; ++length)
    {
        for (std::size_t left = 0; left + length <= n; ++left)
        {
            const std::size_t right = left + length - 1;

            if (text[left] != text[right])
            {
                cache[index(left, right)] = false;
                continue;
            }

            if (length == 2)
            {
                cache[index(left, right)] = true;
            }
            else
            {
                cache[index(left, right)] = cache[index(left + 1, right - 1)];
            }

            if (cache[index(left, right)] && length > best_length)
            {
                best_start = left;
                best_length = length;
            }
        }
    }

    return text.substr(best_start, best_length);
}

int main()
{
    assert(longest_palindrome("") == "");
    assert(longest_palindrome("a") == "a");
    assert(longest_palindrome("aa") == "aa");
    assert(longest_palindrome("aba") == "aba");
    assert(longest_palindrome("abba") == "abba");
    assert(longest_palindrome("babad") == "bab" || longest_palindrome("babad") == "aba");
    assert(longest_palindrome("cbbd") == "bb");
    assert(longest_palindrome("forgeeksskeegfor") == "geeksskeeg");

    std::string text;
    std::getline(std::cin, text);

    std::cout << longest_palindrome(text) << '\n';
}