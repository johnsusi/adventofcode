#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct Body {
    int x, y, z, vx = 0, vy = 0, vz = 0;
    Body(int x, int y, int z) : x(x), y(y), z(z) {}
};

int potential(const Body& body)
{
    return std::abs(body.x) + std::abs(body.y) + std::abs(body.z);
}

int kinetic(const Body& body)
{
    return std::abs(body.vx) + std::abs(body.vy) + std::abs(body.vz);
}

auto eq(const std::vector<Body>& lhs, const std::vector<Body>& rhs)
{
    auto n = lhs.size();
    if (rhs.size() != n) return std::tuple { false, false, false };
    bool x = true, y = true, z = true;
    for (int i = 0;i < n;++i)
    {
        if (lhs[i].x != rhs[i].x || lhs[i].vx != rhs[i].vx) x = false;
        if (lhs[i].y != rhs[i].y || lhs[i].vy != rhs[i].vy) y = false;
        if (lhs[i].z != rhs[i].z || lhs[i].vz != rhs[i].vz) z = false;
    }
    return std::tuple { x, y, z};
}

void simulate(std::vector<Body>& moons)
{
    for (int j = 0; j < moons.size(); ++j) {
        for (int k = j + 1; k < moons.size(); ++k) {
            auto dx = std::clamp(moons[k].x - moons[j].x, -1, 1);
            auto dy = std::clamp(moons[k].y - moons[j].y, -1, 1);
            auto dz = std::clamp(moons[k].z - moons[j].z, -1, 1);
            moons[j].vx += dx;
            moons[k].vx -= dx;
            moons[j].vy += dy;
            moons[k].vy -= dy;
            moons[j].vz += dz;
            moons[k].vz -= dz;
        }
    }

    for (auto& moon : moons) {
        moon.x += moon.vx;
        moon.y += moon.vy;
        moon.z += moon.vz;
    }
}

auto part1(std::vector<Body> moons)
{
    for (int i = 0; i < 1000; ++i) simulate(moons);
    int result = 0;
    for (auto moon : moons) result += potential(moon) * kinetic(moon);
    return result;
}

auto part2(std::vector<Body> moons)
{

    auto o = moons;
    long dx = 0, dy = 0, dz = 0;

    for (long i = 1;dx == 0||dy == 0 || dz == 0;++i)
    {
        simulate(moons);
        auto [x, y, z] = eq(o, moons);
        if (dx == 0 && x) dx = i;
        if (dy == 0 && y) dy = i;
        if (dz == 0 && z) dz = i;
    }

    auto d = std::gcd(dx, dy);
    auto da = dx / d;
    auto db = da * dy / d;
    long i = dx;
    while ((i % dy) != 0) i += da;
    while ((i % dz) != 0) i += db;
    return i;
}

int main()
{
    std::ifstream infile("data.txt");
    std::vector<Body> moons;
    for (std::string row; std::getline(infile, row);) {
        int x, y, z;
        std::stringstream parser(row);
        char axis, delim;
        assert(parser >> delim >> axis >> delim >> x);
        assert(axis == 'x' && delim == '=');
        assert(parser >> delim >> axis >> delim >> y);
        assert(axis == 'y' && delim == '=');
        assert(parser >> delim >> axis >> delim >> z);
        assert(axis == 'z' && delim == '=');
        moons.emplace_back(x, y, z);
    }
    std::cout << part1(moons) << std::endl; // 7077
    std::cout << part2(moons) << std::endl; // 402951477454512
    return 0;
}
