#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <common/set.hpp>

int main()
{

    std::ifstream infile("data.txt");

    int part1 = 0, part2 = 0;

    for (std::string line; std::getline(infile, line);) {

        bool first = true;
        std::string yes_union, yes_intersection;

        do {
            std::sort(line.begin(), line.end());
            set_union(yes_union, line);
            if (first) yes_intersection = line;
            else set_intersect(yes_intersection, line);
            first = false;
        } while (std::getline(infile, line) && !line.empty());

        part1 += yes_union.size();
        part2 += yes_intersection.size();
    }

    std::cout << part1 << std::endl; // 6726
    std::cout << part2 << std::endl; // 3316

    return 0;
}
