#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto parse(std::istream&& in)
{
    int w = 0, h = 0;
    std::string s;
    for (std::string line; std::getline(in, line);) {
        s.append(line);
        w = line.size();
        h += 1;
    }
    return std::make_tuple(s, w, h);
}

bool is_empty(const std::string& s, int x, int y, int w, int h, int dx, int dy, bool line = false)
{
    do {
        x += dx;
        y += dy;
        if (x < 0 || y < 0 || x >= w || y >= h) return true;
        if (s[x + y * w] == 'L') return true;
        if (s[x + y * w] == '#') return false;
    } while (line);
    return true;
}

auto eval(std::string s, int w, int h, bool part2)
{
    std::vector<std::pair<int, int>> directions{
        {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
    auto occupy_rule = [&](std::string s, int x, int y, int w, int h) {
        auto is_empty = [&](auto d) { return ::is_empty(s, x, y, w, h, d.first, d.second, part2); };
        return std::all_of(directions.begin(), directions.end(), is_empty);
    };

    auto leave_rule = [&](std::string s, int x, int y, int w, int h) {
        auto is_occupied = [&](auto d) {
            return !is_empty(s, x, y, w, h, d.first, d.second, part2);
        };
        return std::count_if(directions.begin(), directions.end(), is_occupied) >= (part2 ? 5 : 4);
    };

    while (true) {
        std::string r = s;
        for (int y = 0; y < h; ++y)
            for (int x = 0; x < w; ++x)
                if (s[x + y * w] == 'L' && occupy_rule(s, x, y, w, h))
                    r[x + y * w] = '#';
                else if (s[x + y * w] == '#' && leave_rule(s, x, y, w, h))
                    r[x + y * w] = 'L';
        if (r == s) return std::count(r.begin(), r.end(), '#');
        s = r;
    }
}

int main()
{
    auto [s, w, h] = parse(std::ifstream("data.txt"));
    std::cout << eval(s, w, h, false) << std::endl; // 2113
    std::cout << eval(s, w, h, true) << std::endl;  // 1865
    return 0;
}
