#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::string bytes_to_hex(const std::vector<std::uint8_t>& bytes)
{
    std::stringstream stream;
    stream << std::hex << std::right << std::setfill('0');

    for (std::uint8_t byte : bytes)
    {
        stream << std::setw(2) << static_cast<int>(byte);
    }

    return stream.str();
}

inline std::uint8_t hex_digit_to_value(char c)
{
    if (c >= '0' && c <= '9')
    {
        return static_cast<std::uint8_t>(c - '0');
    }

    if (c >= 'a' && c <= 'f')
    {
        return static_cast<std::uint8_t>(10 + (c - 'a'));
    }

    throw std::invalid_argument("invalid digit");
}

inline std::vector<std::uint8_t> hex_to_bytes(const std::string& hex)
{
    if ((hex.size() & 1U) != 0U)
    {
        throw std::invalid_argument("hex string must have even length");
    }

    std::vector<std::uint8_t> bytes;
    bytes.reserve(hex.size() / 2);

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        const std::uint8_t high = hex_digit_to_value(hex[i]);
        const std::uint8_t low  = hex_digit_to_value(hex[i + 1]);

        bytes.push_back(static_cast<std::uint8_t>((high << 4) | low));
    }

    return bytes;
}
