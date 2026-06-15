
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

static uint32_t RSHash(std::string_view s)
{
    uint32_t b = 378551u;
    uint32_t a = 63689u;
    uint32_t h = 0u;
    for (unsigned char c : s) {
        h = h * a + c;
        a = a * b;
    }
    return h;
}

static uint32_t JSHash(std::string_view s)
{
    uint32_t h = 1315423911u;
    for (unsigned char c : s)
    {
        h ^= ((h << 5) + c + (h >> 2));
    }
    return h;
}

static uint32_t PJWHash(std::string_view s)
{
    const uint32_t Bits = static_cast<uint32_t>(sizeof(uint32_t) * 8u);
    const uint32_t ThreeQuarters = (Bits * 3u) / 4u;
    const uint32_t OneEighth = Bits / 8u;
    const uint32_t HighBits = 0xFFFFFFFFu << (Bits - OneEighth);

    uint32_t h = 0u;
    uint32_t test = 0u;

    for (unsigned char c : s)
    {
        h = (h << OneEighth) + c;
        test = h & HighBits;
        if (test != 0u)
        {
            h = ((h ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }
    return h;
}

static uint32_t ELFHash(std::string_view s)
{
    uint32_t h = 0u;
    uint32_t x = 0u;
    for (unsigned char c : s)
    {
        h = (h << 4) + c;
        x = h & 0xF0000000u;
        if (x != 0u) h ^= (x >> 24);
        h &= ~x;
    }
    return h;
}

static uint32_t BKDRHash(std::string_view s)
{
    const uint32_t seed = 131u;
    uint32_t h = 0u;
    for (unsigned char c : s)
    {
        h = (h * seed) + c;
    }
    return h;
}

static uint32_t SDBMHash(std::string_view s)
{
    uint32_t h = 0u;
    for (unsigned char c : s)
    {
        h = c + (h << 6) + (h << 16) - h;
    }
    return h;
}

static uint32_t DJBHash(std::string_view s)
{
    uint32_t h = 5381u;
    for (unsigned char c : s)
    {
        h = ((h << 5) + h) + c;
    }
    return h;
}

static uint32_t DEKHash(std::string_view s)
{
    uint32_t h = static_cast<uint32_t>(s.size());
    for (unsigned char c : s) {
        h = ((h << 5) ^ (h >> 27)) ^ c;
    }
    return h;
}

static uint32_t APHash(std::string_view s)
{
    uint32_t h = 0xAAAAAAAAu;
    uint32_t i = 0u;
    for (unsigned char c : s)
    {
        if ((i & 1u) == 0u)
        {
            h ^= ((h << 7) ^ (static_cast<uint32_t>(c) * (h >> 3)));
        }
        else
        {
            h ^= (~((h << 11) + (static_cast<uint32_t>(c) ^ (h >> 5))));
        }
        ++i;
    }
    return h;
}

using HashFn = uint32_t(*)(std::string_view);

static std::vector<std::string> read_lines(const std::string& path)
{
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: cannot open file: " << path << "\n";
        std::exit(1);
    }
    std::vector<std::string> lines;
    std::string s;
    while (std::getline(in, s)) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
        if (!s.empty()) lines.push_back(s);
    }
    return lines;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: ./hash_lab input.txt [maxN=all] [step=1000] [buckets=100003] > out.csv\n";
        return 2;
    }

    const std::string path = argv[1];
    std::size_t maxN = std::numeric_limits<std::size_t>::max();
    std::size_t step = 1000;
    std::size_t buckets = 100003;

    if (argc >= 3) maxN = static_cast<std::size_t>(std::stoull(argv[2]));
    if (argc >= 4) step = static_cast<std::size_t>(std::stoull(argv[3]));
    if (argc >= 5) buckets = static_cast<std::size_t>(std::stoull(argv[4]));
    if (step == 0 || buckets == 0)
    {
        std::cerr << "Error: step and buckets must be > 0\n";
        return 2;
    }

    auto data = read_lines(path);
    if (data.empty())
    {
        std::cerr << "Error: no non-empty lines in input\n";
        return 2;
    }

    if (maxN == std::numeric_limits<std::size_t>::max() || maxN > data.size()) maxN = data.size();
    maxN = (maxN / step) * step; // чтобы последняя точка ровно попадала в CSV
    if (maxN == 0)
    {
        std::cerr << "Error: maxN becomes 0 (maybe step > number of lines)\n";
        return 2;
    }

    const char* names[] = {"RS","JS","PJW","ELF","BKDR","SDBM","DJB","DEK","AP"};
    const HashFn fns[] = {RSHash, JSHash, PJWHash, ELFHash, BKDRHash, SDBMHash, DJBHash, DEKHash, APHash};
    constexpr std::size_t K = sizeof(fns) / sizeof(fns[0]);

    std::vector<std::vector<uint32_t>> counts(K, std::vector<uint32_t>(buckets, 0u));
    std::vector<std::uint64_t> collisions(K, 0ull);

    auto add_one = [&](std::size_t fn_i, std::string_view s)
    {
        uint32_t h = fns[fn_i](s);
        std::size_t b = static_cast<std::size_t>(h % static_cast<uint32_t>(buckets));
        if (counts[fn_i][b] >= 1u) collisions[fn_i] += 1ull;
        counts[fn_i][b] += 1u;
    };

    std::cout << "N";
    for (auto n : names) std::cout << "," << n;
    std::cout << "\n";

    std::size_t cur = 0;
    for (std::size_t N = step; N <= maxN; N += step)
    {
        for (; cur < N; ++cur)
        {
            std::string_view s = data[cur];
            for (std::size_t i = 0; i < K; ++i) add_one(i, s);
        }

        std::cout << N;
        for (std::size_t i = 0; i < K; ++i) std::cout << "," << collisions[i];
        std::cout << "\n";
    }

    std::size_t best = 0, worst = 0;
    for (std::size_t i = 1; i < K; ++i)
    {
        if (collisions[i] < collisions[best]) best = i;
        if (collisions[i] > collisions[worst]) worst = i;
    }
    std::cerr << "At N=" << maxN << " buckets=" << buckets
              << " BEST=" << names[best] << " collisions=" << collisions[best]
              << " | WORST=" << names[worst] << " collisions=" << collisions[worst]
              << "\n";
    return 0;
}