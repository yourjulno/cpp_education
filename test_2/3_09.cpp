#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>

class IPv4
{
private:
    std::array<std::uint8_t, 4> data;

public:

    IPv4() : data{0, 0, 0, 0} {}

    IPv4(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d)
        : data{a, b, c, d} {}

    IPv4(const std::array<std::uint8_t, 4>& arr) : data(arr) {}

    IPv4& operator++()
    {
        for (int i = 3; i >= 0; --i)
        {
            if (data[i] < 255)
            {
                ++data[i];
                break;
            } else
            {
                data[i] = 0;
            }
        }
        return *this;
    }

    IPv4 operator++(int)
    {
        IPv4 temp(*this);
        ++(*this);
        return temp;
    }

    IPv4& operator--()
    {
        for (int i = 3; i >= 0; --i)
        {
            if (data[i] > 0)
            {
                --data[i];
                break;
            } else
            {
                data[i] = 255;
            }
        }
        return *this;
    }

    IPv4 operator--(int)
    {
        IPv4 temp(*this);
        --(*this);
        return temp;
    }

    friend bool operator==(const IPv4& lhs, const IPv4& rhs) {
        return lhs.data == rhs.data;
    }

    friend bool operator!=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<(const IPv4& lhs, const IPv4& rhs) {
        return lhs.data < rhs.data;
    }

    friend bool operator>(const IPv4& lhs, const IPv4& rhs) {
        return rhs < lhs;
    }

    friend bool operator<=(const IPv4& lhs, const IPv4& rhs) {
        return !(rhs < lhs);
    }

    friend bool operator>=(const IPv4& lhs, const IPv4& rhs) {
        return !(lhs < rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const IPv4& ip)
    {
        os << static_cast<int>(ip.data[0]) << '.'
           << static_cast<int>(ip.data[1]) << '.'
           << static_cast<int>(ip.data[2]) << '.'
           << static_cast<int>(ip.data[3]);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, IPv4& ip)
    {
        int a, b, c, d;
        char dot1, dot2, dot3;

        is >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;

        if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
            a >= 0 && a <= 255 &&
            b >= 0 && b <= 255 &&
            c >= 0 && c <= 255 &&
            d >= 0 && d <= 255)
        {
            ip.data[0] = static_cast<std::uint8_t>(a);
            ip.data[1] = static_cast<std::uint8_t>(b);
            ip.data[2] = static_cast<std::uint8_t>(c);
            ip.data[3] = static_cast<std::uint8_t>(d);
        } else {
            is.setstate(std::ios::failbit);
        }

        return is;
    }

    std::array<std::uint8_t, 4> getData() const
    {
        return data;
    }
};

int main() {

    IPv4 ip1;
    IPv4 ip2(192, 168, 1, 1);
    IPv4 ip3(std::array<std::uint8_t, 4>{10, 0, 0, 1});

    std::cout << "ip1: " << ip1 << std::endl;
    std::cout << "ip2: " << ip2 << std::endl;
    std::cout << "ip3: " << ip3 << std::endl;

    IPv4 ip4(192, 168, 1, 254);
    std::cout << "ip4: " << ip4 << std::endl;
    std::cout << "++ip4: " << ++ip4 << std::endl;
    std::cout << "ip4++: " << ip4++ << std::endl;
    std::cout << "after ip4++: " << ip4 << std::endl;

    IPv4 ip5(192, 168, 1, 1);
    std::cout << "ip5: " << ip5 << std::endl;
    std::cout << "--ip5: " << --ip5 << std::endl;
    std::cout << "ip5--: " << ip5-- << std::endl;
    std::cout << "after ip5--: " << ip5 << std::endl;

    IPv4 ip6(192, 168, 1, 1);
    IPv4 ip7(192, 168, 1, 2);
    std::cout << ip6 << " == " << ip7 << ": " << (ip6 == ip7) << std::endl;
    std::cout << ip6 << " < " << ip7 << ": " << (ip6 < ip7) << std::endl;

    std::stringstream ss;
    ss << "192.168.1.100";
    IPv4 ip8;
    ss >> ip8;

    IPv4 ip9(255, 255, 255, 255);
    std::cout << "max IP: " << ip9 << std::endl;
    std::cout << "after incr: " << ++ip9 << std::endl;

    IPv4 ip10(0, 0, 0, 0);
    std::cout << "min IP: " << ip10 << std::endl;
    std::cout << "after decr: " << --ip10 << std::endl;

    return 0;
}