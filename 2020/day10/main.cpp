#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main()
{

    auto infile = std::ifstream("data.txt");
    auto source = std::string(std::istreambuf_iterator<char>(infile), {});
    auto parser = std::istringstream(source);

    std::vector<int> data = {0};
    for (int code; parser >> code; data.push_back(code))
        ;

    std::sort(data.begin(), data.end());
    data.push_back(data[data.size() - 1] + 3);

    int joltage = 0;
    std::vector<int> diff;
    for (int i = 0; i < data.size(); ++i) {
        diff.push_back(data[i] - joltage);
        joltage = data[i];
    }
    int c1 = std::count(diff.begin(), diff.end(), 1);
    int c2 = std::count(diff.begin(), diff.end(), 2);
    int c3 = std::count(diff.begin(), diff.end(), 3);

    std::cout << c1 * c3 << std::endl; // 2368

    // The groups of 1:s can have 2, 4 and 7 permutations each independent of all other groups
    // 11..1111.111.1111.11.1111..111...1111...1.1...111..11.111.111.11..11.111..111..1111.1111...1111..111.
    long count = 1;
    for (int i = 0, j = 0; i < diff.size(); ++i) {
        if (diff[i] == 1)
            ++j;
        else {
            if (j == 2) count *= 2;
            if (j == 3) count *= 4;
            if (j == 4) count *= 7;
            j = 0;
        }
    }

    std::cout << count << std::endl; // 1727094849536

    return 0;
}
