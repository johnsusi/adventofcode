#include <fstream>
#include <iostream>
#include <vector>

uint64_t part1(std::vector<uint64_t> numbers)
{
    for (int i = 0; i < numbers.size(); ++i)
        for (int j = i + 1; j < numbers.size(); ++j)
            if (numbers[i] + numbers[j] == 2020) return numbers[i] * numbers[j];
    return 0;
}

uint64_t part2(std::vector<uint64_t> numbers)
{
      for (int i = 0; i < numbers.size(); ++i)
        for (int j = i + 1; j < numbers.size(); ++j)
            for (int k = j + 1; k < numbers.size(); ++k)
                if (numbers[i] + numbers[j] + numbers[k] == 2020)
                    return numbers[i] * numbers[j] * numbers[k];
  return 0;
}

int main()
{

    std::ifstream infile("data.txt");

    std::vector<uint64_t> numbers;
    for (uint64_t value; infile >> value;) numbers.push_back(value);

    std::cout << part1(numbers) << std::endl; // 805731
    std::cout << part2(numbers) << std::endl; // 192684960

    return 0;
}
