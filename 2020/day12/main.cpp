#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void rotate(int& x, int& y, int deg)
{
    while (deg < 0) deg += 360;
    for (int i = 0; i < deg; i += 90) {
        std::swap(x, y);
        x = -x;
    }
}

int part1(std::vector<std::pair<char, int>> codes)
{
    int x = 0, y = 0, wp_x = 1, wp_y = 0;
    for (auto [code, val] : codes) {
        switch (code) {
        case 'N': y += val; break;
        case 'S': y -= val; break;
        case 'E': x += val; break;
        case 'W': x -= val; break;
        case 'L': rotate(wp_x, wp_y, val); break;
        case 'R': rotate(wp_x, wp_y, -val); break;
        case 'F':
            x += val * wp_x;
            y += val * wp_y;
            break;
        }
    }
    return std::abs(x) + std::abs(y);
}

int part2(std::vector<std::pair<char, int>> codes)
{
    int x = 0, y = 0, wp_x = 10, wp_y = 1;
    for (auto [code, val] : codes) {
        switch (code) {
        case 'N': wp_y += val; break;
        case 'S': wp_y -= val; break;
        case 'E': wp_x += val; break;
        case 'W': wp_x -= val; break;
        case 'L': rotate(wp_x, wp_y, val); break;
        case 'R': rotate(wp_x, wp_y, -val); break;
        case 'F':
            x += val * wp_x;
            y += val * wp_y;
            break;
        }
    }
    return std::abs(x) + std::abs(y);
}

int main()
{
    auto infile = std::ifstream("data.txt");
    auto source = std::string(std::istreambuf_iterator<char>(infile), {});
    auto parser = std::istringstream(source);
    std::vector<std::pair<char, int>> codes;
    for (std::string code; parser >> code;) codes.emplace_back(code[0], std::stoi(code.substr(1)));

    std::cout << part1(codes) << std::endl; // 757
    std::cout << part2(codes) << std::endl; // 51249
    return 0;
}
