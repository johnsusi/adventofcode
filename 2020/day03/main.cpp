#include <fstream>
#include <iostream>
#include <string>
#include <vector>

long traverse(int dx, int dy, int w, int h, const std::vector<std::string>& map)
{
    long tree = 0;
    for (int x = 0, y = 0; y < h; x += dx, y += dy) {
        const auto& row = map[y];
        switch (row[x % w]) {
        case '#': ++tree; break;
        case '.': break;
        default: std::terminate();
        }
    }
    return tree;
}

int main()
{
    std::ifstream infile("data.txt");

    std::vector<std::string> map;
    for (std::string line; std::getline(infile, line);) map.push_back(line);

    int w = map[0].size(), h = map.size();
    long r11 = traverse(1, 1, w, h, map);
    long r31 = traverse(3, 1, w, h, map);
    long r51 = traverse(5, 1, w, h, map);
    long r71 = traverse(7, 1, w, h, map);
    long r12 = traverse(1, 2, w, h, map);

    std::cout << r31 << std::endl;                         // 259
    std::cout << r11 * r31 * r51 * r71 * r12 << std::endl; // 2224913600

    return 0;
}
