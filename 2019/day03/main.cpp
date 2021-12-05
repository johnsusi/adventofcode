#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>

struct Point
{
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator<(const Point& other) const
    {
        return std::tie(x,y) < std::tie(other.x, other.y);
    }
};

using Map = std::map<Point, int>;

void draw(Map& map, std::string wire)
{
    std::stringstream parser(wire);
    char dir, delim;
    int steps, dist = 0;
    auto pos = Point { 0, 0 };
    while (parser >> dir >> steps)
    {
        int dx = 0, dy = 0;
        switch (dir)
        {
            case 'R': dx = 1; break;
            case 'U': dy = 1; break;
            case 'L': dx = -1; break;
            case 'D': dy = -1; break;
            default: std::terminate();
        }

        for (;steps > 0;--steps)
        {
            pos.x += dx;
            pos.y += dy;
            ++dist;
            if (!map.contains(pos))
                map[pos] = dist;
        }

        if (!(parser >> delim)) break;
    }
}

auto part1(const Map& map1, const Map& map2)
{
    int result = std::numeric_limits<int>::max();
    for (auto [k, v] : map1)
    {
        if (map2.contains(k))
        {
            int dist = std::abs(k.x) + std::abs(k.y);
            result = std::min(result, dist);
        }
    }
    return result;
}

auto part2(const Map& map1, const Map& map2)
{
    int result = std::numeric_limits<int>::max();
    for (auto [k, v] : map1)
    {
        if (map2.contains(k))
        {
            int dist = v + map2.at(k);
            result = std::min(result, dist);
        }
    }
    return result;
}

int main()
{
    std::ifstream infile("data.txt");
    std::string wire1, wire2;
    std::getline(infile, wire1);
    std::getline(infile, wire2);

    Map map1, map2;
    draw(map1, wire1);
    draw(map2, wire2);

    std::cout << part1(map1, map2) << std::endl; // 529
    std::cout << part2(map1, map2) << std::endl; // 20386

    return 0;
}
