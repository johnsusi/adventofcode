#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int main()
{
    std::ifstream infile("data.txt");
    std::vector<int> ids;
    for (std::string line; std::getline(infile, line);) {
        assert(line.size() == 10);
        int row = 0, rowsize = 128, col = 0, colsize = 8;
        for (auto c : line) {
            switch (c) {
            case 'F': rowsize /= 2; break;
            case 'B': row += rowsize /= 2; break;
            case 'L': colsize /= 2; break;
            case 'R': col += colsize /= 2; break;
            }
        }
        int id = row * 8 + col;
        ids.push_back(id);
    }

    std::sort(ids.begin(), ids.end());

    auto it1 = std::max_element(ids.begin(), ids.end());
    std::cout << *it1 << std::endl; // 996

    auto it2 = std::adjacent_find(ids.begin(), ids.end(), [](auto a, auto b) { return b - a > 1; });
    std::cout << 1 + *it2 << std::endl; // 671

    return 0;
}
