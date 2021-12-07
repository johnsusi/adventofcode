#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <common/string.hpp>

// + 0 - 0 + 0 - 0 + 0 - 0 + 0 - 0
// 0 + + 0 0 - - 0 0 + + 0 0 - - 0
// 0 0 + + + 0 0 0 - - - 0 0 0 + +
// 0 0 0 + + + + 0 0 0 0 - - - - 0
// 0 0 0 0 + + + + + 0 0 0 0 0 - -
// 0 0 0 0 0 + + + + + + 0 0 0 0 0
// 0 0 0 0 0 0 + + + + + + + 0 0 0
// 0 0 0 0 0 0 0 + + + + + + + + 0
// 0 0 0 0 0 0 0 0 + + + + + + + + <-- bottom half is simple and that is where
// 0 0 0 0 0 0 0 0 0 + + + + + + +     the message is in part 2
// 0 0 0 0 0 0 0 0 0 0 + + + + + +
// 0 0 0 0 0 0 0 0 0 0 0 + + + + +
// 0 0 0 0 0 0 0 0 0 0 0 0 + + + +
// 0 0 0 0 0 0 0 0 0 0 0 0 0 + + +
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 + +
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 +

void fft2(const std::string& input, std::string& result)
{
    int sum = 0, n = input.size();
    for (int i = n - 1;i >= n / 2; --i)
    {
        sum += input[i] - '0';
        result[i] = ((char)(std::abs(sum) % 10) + '0');
    }
}

auto fft(const std::string& input)
{
    const int n = input.size();
    std::string result;
    result.resize(input.size());

    for (int i = 0;i < n / 2; ++i)
    {
        int sum = 0;
        for (int j = -1;j < n;)
        {
            j += i+1;
            for (int k = 0;j < n && k < i+1;++k)
                sum += input[j++] - '0';
            j += i+1;
            for (int k = 0;j < n && k < i+1;++k)
                sum -= input[j++] - '0';
        }
        result[i] = ((char)(std::abs(sum) % 10) + '0');
    }

    fft2(input, result);
    return result;
}

auto part1(std::string input)
{
    for (int i = 0;i < 100; ++i)
        input = fft(input);
    return input.substr(0, 8);
}

auto part2(std::string input)
{
    auto offset = std::stoi(input.substr(0, 7));
    input = repeat(input, 10000);
    std::string temp;
    temp.resize(input.size());

    for (int i = 0;i < 100;++i)
    {
        fft2(input, temp);
        input = temp;
    }

    return input.substr(offset, 8);
}

int main()
{
    assert(fft("12345678") == "48226158");
    assert(fft("48226158") == "34040438");
    assert(fft("34040438") == "03415518");
    assert(fft("03415518") == "01029498");

    assert(part1("80871224585914546619083218645595") == "24176176");
    assert(part1("19617804207202209144916044189917") == "73745418");
    assert(part1("69317163492948606335995924319873") == "52432133");

    assert(part2("03036732577212944063491565474664") == "84462026");
    assert(part2("02935109699940807407585447034323") == "78725270");
    assert(part2("03081770884921959731165446850517") == "53553731");

    std::ifstream infile("data.txt");
    std::string input;
    std::getline(infile, input);
    std::cout << part1(input) << std::endl; // 96136976
    std::cout << part2(input) << std::endl; // 85600369
    return 0;
}
