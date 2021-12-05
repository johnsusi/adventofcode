#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct line_t {
    int x1, y1, x2, y2;
    line_t(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    int dx() const { return x1 < x2 ? 1 : x1 > x2 ? -1 : 0; }
    int dy() const { return y1 < y2 ? 1 : y1 > y2 ? -1 : 0; }
};

auto parse(std::ifstream& infile)
{
    std::vector<line_t> lines;
    for (std::string line; std::getline(infile, line); ) {
        std::stringstream parser(line);
        int x1, y1, x2, y2;
        char delim;
        if (parser >> x1 >> delim >> y1 >> delim >> delim >> x2 >> delim >> y2) {
            lines.emplace_back(x1, y1, x2, y2);
        }
    }
    return lines;
}

auto dimensions(const std::vector<line_t>& lines)
{
    int xMax = std::numeric_limits<int>::min();
    int yMax = std::numeric_limits<int>::min();
    for (auto [x1, y1, x2, y2] : lines)
    {
        xMax = std::max(xMax, x2);
        yMax = std::max(yMax, y2);
    }
    return std::tuple {xMax+1, yMax +1};
}

auto count(const std::vector<int>& map)
{
    int count = 0;
    for (auto x : map)
        if (x > 1) ++count;
    return count;
}

void draw(std::vector<int>& map, int x, int y, int x2, int y2, int dx, int dy, int w)
{
    for (; x != x2 || y != y2; x += dx, y += dy) map[x + y * w] += 1;
    map[x2 + y2 * w] += 1;
}

auto part1(const std::vector<line_t>& lines, int w, int h)
{
    std::vector<int> map(w*h, 0);
    for (auto l : lines) {
        int x = l.x1, y = l.y1, dx = l.dx(), dy = l.dy();
        if (dx == 0 || dy == 0) draw(map, x, y, l.x2, l.y2, dx, dy, w);
    }
    return count(map);
}

auto part2(const std::vector<line_t>& lines, int w, int h)
{
    std::vector<int> map(w*h, 0);
    for (auto l : lines) {
        int x = l.x1, y = l.y1, dx = l.dx(), dy = l.dy();
        draw(map, x, y, l.x2, l.y2, dx, dy, w);
    }
    return count(map);
}

int main()
{
    std::ifstream infile("data.txt");
    auto lines = parse(infile);
    auto [w,h] = dimensions(lines);
    std::cout << part1(lines, w, h) << std::endl; // 6225
    std::cout << part2(lines, w, h) << std::endl; // 22116
    return 0;
}