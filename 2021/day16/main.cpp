#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto& infile)
{
    std::string result;
    std::getline(infile, result);
    return result;
}

auto hex(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else
        return (c - 'A') + 10;
}
int read_bits(const auto& s, int offset, int length, int* next)
{
    int result = 0;
    for (int end = offset + length; offset < end; ++offset) {
        result <<= 1;
        int a = offset / 4;
        int b = 3 - (offset % 4);
        result |= (hex(s[a]) >> b) & 1;
    }
    if (next) *next = offset;
    return result;
}

auto read_packet_header(const auto& s, int i, int* next)
{
    int ver = read_bits(s, i, 3, next);
    int id  = read_bits(s, i + 3, 3, next);
    return std::pair{ver, id};
}

auto read_literal_value(const auto& s, int i, int* next)
{

    std::uint64_t result = 0;
    while (true) {
        auto digit = read_bits(s, i, 5, &i);
        result *= 16;
        result += (digit % 16);
        if (!(digit / 16)) break;
    }
    if (next) *next = i;
    return result;
}

int sum_packet_versions(const std::string& s, int i, int* next)
{

    if (i+6 >= s.size()*4)
    {
        return 0;
    }
    int result = 0;
    auto [ver, id] = read_packet_header(s, i, &i);
    result += ver;
    if (id == 4)
        read_literal_value(s, i, &i);
    else {
        int length_type = read_bits(s, i, 1, &i);
        if (length_type > 0) {
            int length = read_bits(s, i, 11, &i);
            for (int j = 0; j < length; ++j) result += sum_packet_versions(s, i, &i);
        }
        else {
            int length = read_bits(s, i, 15, &i);
            for (auto end = i + length;i < end;)
                result += sum_packet_versions(s, i, &i);
        }
    }
    if (next) *next = i;
    return result;
}


std::uint64_t eval(const std::string& s, int i, int* next)
{

    if (i+6 >= s.size()*4)
    {
        return 0;
    }
    std::uint64_t result = 0;
    auto [ver, id] = read_packet_header(s, i, &i);
    // std::cout << ver << ", " << id << ", " << i << std::endl;
    // result += ver;
    if (id == 4)
        result = read_literal_value(s, i, &i);
    else {
        int length_type = read_bits(s, i, 1, &i);
        // std::cout << length_type << std::endl;
        std::vector<std::uint64_t> vals;
        if (length_type > 0) {
            int length = read_bits(s, i, 11, &i);
            for (int j = 0; j < length; ++j) vals.push_back(eval(s, i, &i));
        }
        else {
            int length = read_bits(s, i, 15, &i);
            for (auto end = i + length;i < end;)
                vals.push_back(eval(s, i, &i));
        }

        switch (id) {
            case 0:
                std::cout << "+";
                for (auto val : vals)
                    result += val;
                break;
            case 1:
                std::cout << "*";
                result = 1;
                for (auto val : vals)
                    result *= val;
                break;
            case 2:
                result = *std::min_element(vals.begin(), vals.end());
                break;
            case 3:
                result = *std::max_element(vals.begin(), vals.end());
                break;
            case 5:
                std::cout << ">";
                result = vals[0] > vals[1] ? 1 : 0;
                break;
            case 6:
                std::cout << "<";
                result = vals[0] < vals[1] ? 1 : 0;
                break;
            case 7:
                std::cout << "=";
                result = vals[0] == vals[1] ? 1 : 0;
                break;

        }
    }
    if (next) *next = i;
    std::cout << result << std::endl;
    return result;
}
auto part1(auto input)
{
    return sum_packet_versions(input, 0, nullptr);
}

auto part2(auto input)
{
    return eval(input, 0, nullptr);
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
    std::cout << part1(input) << std::endl; //
    std::cout << part2(input) << std::endl; //
    return 0;
}
