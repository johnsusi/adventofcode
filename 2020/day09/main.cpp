#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

bool checksum(const std::vector<int>& data, int i, int preamble)
{
    for (int j = i - 1; j >= i - preamble; --j)
        for (int k = j - 1; k >= i - preamble; --k)
            if (data[i] == data[j] + data[k]) return true;
    return false;
}

long part1(const std::vector<int>& data, int preamble)
{
    for (int i = preamble; i < data.size(); ++i)
        if (!checksum(data, i, preamble)) return data[i];
    assert(false);
}

long part2(const std::vector<int>& data, long invalid)
{
    for (auto it = data.begin(); it != data.end(); ++it) {
        long sum = 0;
        auto jt  = it;
        while (sum < invalid && jt != data.end()) sum += *jt++;
        if (sum == invalid) return *std::min_element(it, jt) + *std::max_element(it, jt);
    }
    assert(false);
}

int main()
{
    int preamble = 25;
    auto infile  = std::ifstream("data.txt");
    auto source  = std::string(std::istreambuf_iterator<char>(infile), {});
    auto parser  = std::istringstream(source);

    std::vector<int> data;
    for (int code; parser >> code; data.push_back(code))
        ;

    long result1 = part1(data, preamble);
    long result2 = part2(data, result1);

    std::cout << result1 << std::endl; // 542529149
    std::cout << result2 << std::endl; // 75678618

    return 0;
}
