#pragma once
#include <algorithm>
#include <functional>
#include <limits>
#include <map>
#include <tuple>

template <typename T> auto bounds(std::map<std::pair<int, int>, T> m)
{
    auto x1 = std::numeric_limits<int>::max();
    auto y1 = std::numeric_limits<int>::max();
    auto x2 = std::numeric_limits<int>::min();
    auto y2 = std::numeric_limits<int>::min();
    for (auto [k, v] : m) {
        auto [x, y] = k;
        x1          = std::min(x1, x);
        y1          = std::min(y1, y);
        x2          = std::max(x2, x);
        y2          = std::max(y2, y);
    }

    return std::tuple{x1, y1, x2, y2};
}

struct Map {
    std::map<std::pair<int, int>, int> data;

    std::function<char(int)> to_char = [](int v) { return (char)v; };

    int& operator[](std::pair<int, int> index) { return data[index]; }

    int operator[](std::pair<int, int> index) const { return data.at(index); }

    std::vector<int> get(std::initializer_list<std::pair<int, int>> indexes) const
    {
        std::vector<int> result;
        ;
        for (auto index : indexes) {
            if (auto it = data.find(index); it != data.end())
                result.push_back(it->second);
            else
                result.push_back(0);
        }
        return result;
    }

    std::string to_string() const
    {
        auto [x1, y1, x2, y2] = bounds(data);
        auto w                = x2 - x1 + 1;
        auto h                = y2 - y1 + 1;
        std::cout << w << "x" << h << " (" << x1 << ", " << y1 << " -> " << x2 << ", " << y2 << ")"
                  << std::endl;
        std::string result(w * h + h, ' ');
        for (int i = 0; i < h; ++i) result[w + i * w + i] = '\n';
        for (auto [k, v] : data) {
            auto [x, y]                                                  = k;
            result[x - x1 + (h - 1 - (y - y1)) * w + (h - 1 - (y - y1))] = to_char(v);
        }
        return result;
    }
};

auto operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    return std::pair{lhs.first + rhs.first, lhs.second + rhs.second};
}
auto& operator+=(std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

auto rotate(const std::pair<int, int>& pos, int deg)
{
    while (deg < 0) deg += 360;
    while (deg >= 260) deg -= 360;
    assert(deg == 0 || deg == 90 || deg == 180 || deg == 270);
    auto [x, y] = pos;
    for (int i = 0; i < deg; i += 90) {
        std::swap(x, y);
        y = -y;
    }
    return std::pair{x, y};
}
