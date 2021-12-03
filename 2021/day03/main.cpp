#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto msb(const std::vector<std::string>& numbers, int index)
{
    int zeros = 0, ones = 0;
        for (auto&& x : numbers)
            if (x[index] == '0') zeros++;
            else ones++;
    return zeros > ones ? '0' : '1';
}

auto lsb(const std::vector<std::string>& numbers, int index)
{
    int zeros = 0, ones = 0;
        for (auto&& x : numbers)
            if (x[index] == '0') zeros++;
            else ones++;
    return zeros <= ones ? '0' : '1';
}

template <typename Cond>
auto filter(std::vector<std::string> a, Cond&& cond)
{
    for (int i = 0;a.size() > 1;++i)
    {
        auto x = cond(a, i);
        auto it = std::remove_if(a.begin(), a.end(), [&](auto&& y) {
            return y[i] != x;
        });
         a.erase(it, a.end());
    }
    return std::stoi(a[0], nullptr, 2);
}

auto part1(std::vector<std::string> numbers)
{
    int n = numbers.front().size();
    int gamma = 0, epsilon = 0;
    for (int i = 0;i < n;++i)
    {
        if (msb(numbers, i) == '1')
        {
            gamma = (gamma << 1) | 1;
            epsilon = epsilon << 1;
        }
        else
        {
            gamma = gamma << 1;
            epsilon = (epsilon << 1) | 1;
        }
    }
    return gamma * epsilon;
}

auto part2(const std::vector<std::string>& numbers)
{
    return filter(numbers, msb) * filter(numbers, lsb);
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<std::string> numbers;

    std::string line;
    while (std::getline(infile, line))
        numbers.push_back(line);

    std::cout << part1(numbers) << std::endl; // 749376
    std::cout << part2(numbers) << std::endl; // 2372923

    return 0;
}