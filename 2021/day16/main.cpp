#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto parse(auto &infile)
{
    std::string result;
    std::getline(infile, result);
    return result;
}

auto from_hex(char c)
{
    return (c >= '0' && c <= '9')
        ? c - '0'
        : c - 'A' + 10;
}

int read_bits(const auto &s, int offset, int length, int *next)
{
    int result = 0;
    for (int end = offset + length; offset < end; ++offset)
    {
        result <<= 1;
        int a = offset / 4;
        int b = 3 - (offset % 4);
        result |= (from_hex(s[a]) >> b) & 1;
    }
    if (next)
        *next = offset;
    return result;
}

auto read_literal_value(const auto &s, int i, int *next)
{
    auto result = 0ull;
    while (true)
    {
        auto digit = read_bits(s, i, 5, &i);
        result *= 16;
        result += (digit % 16);
        if (!(digit / 16))
            break;
    }
    if (next) *next = i;
    return result;
}

std::uint64_t eval(const std::string &s, int i = 0, int *next = nullptr, std::uint64_t* checksum = nullptr)
{
    int ver = read_bits(s, i, 3, &i);
    int id = read_bits(s, i, 3, &i);
    if (checksum) *checksum += ver;
    auto params = std::vector<std::uint64_t>{};
    if (id != 4)
    {
        int length_type = read_bits(s, i, 1, &i);
        if (length_type > 0)
        {
            int length = read_bits(s, i, 11, &i);
            for (int j = 0; j < length; ++j)
                params.push_back(eval(s, i, &i, checksum));
        }
        else
        {
            int length = read_bits(s, i, 15, &i);
            for (auto end = i + length; i < end;)
                params.push_back(eval(s, i, &i, checksum));
        }
    }

    if (next) *next = i;
    switch (id) {
        case 0: return std::accumulate(params.begin(), params.end(), 0ull);
        case 1: return std::accumulate(params.begin(), params.end(), 1, std::multiplies<int>());
        case 2: return *std::min_element(params.begin(), params.end());
        case 3: return *std::max_element(params.begin(), params.end());
        case 4: return read_literal_value(s, i, next);
        case 5: return params[0] > params[1] ? 1 : 0;
        case 6: return params[0] < params[1] ? 1 : 0;
        case 7: return params[0] == params[1] ? 1 : 0;
        default: std::terminate();
    }
}

auto part1(auto input)
{
    auto result = 0ull;
    eval(input, 0, nullptr, &result);
    return result;
}

auto part2(auto input)
{
    return eval(input);
}

int main()
{
    assert(part1("8A004A801A8002F478") == 16);
    assert(part1("620080001611562C8802118E34") == 12);
    assert(part1("C0015000016115A2E0802F182340") == 23);
    assert(part1("A0016C880162017C3686B18A3D4780") == 31);
    assert(part2("C200B40A82") == 3);
    assert(part2("04005AC33890") == 54);
    assert(part2("880086C3E88112") == 7);
    assert(part2("CE00C43D881120") == 9);
    assert(part2("D8005AC2A8F0") == 1);
    assert(part2("F600BC2D8F") == 0);
    assert(part2("9C005AC2F8F0") == 0);
    assert(part2("9C0141080250320F1802104A08") == 1);
    std::ifstream infile("data.txt");
    auto input = parse(infile);
    std::cout << part1(input) << std::endl; // 945
    std::cout << part2(input) << std::endl; // 10637009915279
    return 0;
}
