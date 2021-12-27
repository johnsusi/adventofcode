#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile)
{
    std::vector<std::tuple<bool, int, int, int, int, int, int>> result;

    std::regex re("(on|off) x=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)");
    for (std::string line; std::getline(infile, line);)
    {
        std::smatch m;
        assert(std::regex_match(line, m, re));
        bool on = m[1] == "on";
        int x1 = std::stoi(m[2]);
        int x2 = std::stoi(m[3]);
        int y1 = std::stoi(m[4]);
        int y2 = std::stoi(m[5]);
        int z1 = std::stoi(m[6]);
        int z2 = std::stoi(m[7]);
        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(x1, x2);
        if (z1 > z2)
            std::swap(x1, x2);

        result.emplace_back(on, x1, y1, z1, x2, y2, z2);
    }
    return result;
}

using Box = std::tuple<bool, int, int, int, int, int, int>;

void merge(std::vector<Box>& boxes, Box box)
{
    boxes.push_back(box);
}


bool same_size(const std::vector<Box>& boxes)
{
    for (auto it = boxes.begin(); it != boxes.end(); ++it)
    {
        auto [on, x1, y1, z1, x2, y2, z2] = *it;
        for (auto jt = it+1;jt != boxes.end(); ++jt)
        {
            auto [ton, tx1, ty1, tz1, tx2, ty2, tz2] = *jt;
            if (tx1 != x1 || ty1 != y1 || tz1 != z1 || tx2 != x2 || ty2 != y2 || tz2 != z2)
                return false;

        }
    }
    return true;
}

auto splitx(const std::vector<Box> &boxes, int x)
{
    std::vector<Box> left, right;
    for (auto [on, x1, y1, z1, x2, y2, z2] : boxes)
        if (x2 < x) merge(left, {on, x1, y1, z1, x2, y2, z2});
        else if (x1 >= x) merge(right, {on, x1, y1, z1, x2, y2, z2});
        else
        {
            merge(left, {on, x1, y1, z1, x-1, y2, z2});
            merge(right, {on, x, y1, z1, x2, y2, z2});
        }
    return std::pair{left, right};
}

auto splity(const std::vector<Box> &boxes, int y)
{
    std::vector<Box> left, right;
    for (auto [on, x1, y1, z1, x2, y2, z2] : boxes)
        if (y2 < y) merge(left, {on, x1, y1, z1, x2, y2, z2});
        else if (y1 >= y)merge(right, {on, x1, y1, z1, x2, y2, z2});
        else
        {
            merge(left, {on, x1, y1, z1, x2, y-1, z2});
            merge(right, {on, x1, y, z1, x2, y2, z2});
        }
    return std::pair{left, right};
}

auto splitz(const std::vector<Box> &boxes, int z)
{
    std::vector<Box> left, right;
    for (auto [on, x1, y1, z1, x2, y2, z2] : boxes)
        if (z2 < z) merge(left, {on, x1, y1, z1, x2, y2, z2});
        else if (z1 >= z) merge(right, {on, x1, y1, z1, x2, y2, z2});
        else
        {
            merge(left, {on, x1, y1, z1, x2, y2, z-1});
            merge(right, {on, x1, y1, z, x2, y2, z2});
        }
    return std::pair{left, right};
}

auto count(const std::vector<Box> &boxes)
{
    std::set<std::tuple<int,int,int>> lit;
    for (auto [on, x1, y1, z1, x2, y2, z2] : boxes)
    {
        for (int x = x1; x <= x2; ++x)
            for (int y = y1; y <= y2; ++y)
                for (int z = z1; z <= z2; ++z)
                    if (on)
                        lit.emplace(x, y, z);
                    else
                        lit.erase({x, y, z});
    }
    return lit.size();
}

auto part1(auto input)
{
    auto boxes = input;
    boxes = splitx(boxes, -50).second;
    boxes = splitx(boxes, 51).first;
    boxes = splity(boxes, -50).second;
    boxes = splity(boxes, 51).first;
    boxes = splitz(boxes, -50).second;
    boxes = splitz(boxes, 51).first;

    return count(boxes);
}

auto bounds(const std::vector<Box> &boxes)
{
    int xmin = std::numeric_limits<int>::max();
    int ymin = std::numeric_limits<int>::max();
    int zmin = std::numeric_limits<int>::max();
    int xmax = std::numeric_limits<int>::min();
    int ymax = std::numeric_limits<int>::min();
    int zmax = std::numeric_limits<int>::min();

    for (auto [on, x1, y1, z1, x2, y2, z2] : boxes)
    {
        xmin = std::min(xmin, x1);
        ymin = std::min(ymin, y1);
        zmin = std::min(zmin, z1);
        xmax = std::max(xmax, x2);
        ymax = std::max(ymax, y2);
        zmax = std::max(zmax, z2);
    }
    return std::tuple{xmin, ymin, zmin, xmax, ymax, zmax};
}

auto size(int x1, int y1, int z1, int x2, int y2, int z2)
{
    std::uint64_t dx = x2 - x1 + 1;
    std::uint64_t dy = y2 - y1 + 1;
    std::uint64_t dz = z2 - z1 + 1;
    return dx * dy * dz;
}

auto score(const auto& box)
{
    auto [on, x1, y1, z1, x2, y2, z2] = box;
    if (!on) return 0ull;
    std::uint64_t dx = x2 - x1 + 1, dy = y2 - y1 + 1, dz = z2 - z1 + 1;
    return dx * dy * dz;
}

auto part2(auto input)
{
    auto result = 0ull;

    std::vector<std::vector<std::tuple<bool, int, int, int, int, int, int>>> all = { input };

    auto xs = std::vector<int>{};
    auto ys = std::vector<int>{};
    auto zs = std::vector<int>{};
    for (auto [_, x1, y1, z1, x2, y2, z2] : input)
    {
        xs.push_back(x1);
        xs.push_back(x2+1);
        ys.push_back(y1);
        ys.push_back(y2+1);
        zs.push_back(z1);
        zs.push_back(z2+1);
    }
    std::sort(xs.begin(), xs.end()); xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    std::sort(ys.begin(), ys.end()); ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    std::sort(zs.begin(), zs.end()); zs.erase(std::unique(zs.begin(), zs.end()), zs.end());
    std::vector<std::vector<std::tuple<bool, int, int, int, int, int, int>>> temp;

    for (auto boxes : all)
    {

        for (int i = 1;i < xs.size() - 1;++i)
        {
            auto [l, r] = splitx(boxes, xs[i]);
            if (!l.empty())
                temp.push_back(l);
            boxes = r;
        }
        if (!boxes.empty())
            temp.push_back(boxes);
    }
    all = temp;
    temp.clear();

    for (auto boxes : all)
    {
        for (int i = 1;i < ys.size() - 1;++i)
        {
            auto [l, r] = splity(boxes, ys[i]);
            if (!l.empty())
                temp.push_back(l);
            boxes = r;
        }
        if (!boxes.empty())
            temp.push_back(boxes);
    }
    all = temp;
    temp.clear();

    for (auto boxes : all)
    {
        for (int i = 1;i < zs.size() - 1;++i)
        {
            auto [l, r] = splitz(boxes, zs[i]);
            if (!l.empty())
                temp.push_back(l);
            boxes = r;
        }
        if (!boxes.empty())
            temp.push_back(boxes);
    }
    all = temp;
    temp.clear();

    for (auto boxes : all)
    {
        result += score(boxes.back());

    }

    return result;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 582644
    std::cout << part2(input) << std::endl; // 1263804707062415
    return 0;
}
