#include <cassert>
#include <fstream>
#include <iostream>
#include <map>

#include <common/intcode.hpp>
#include <common/string.hpp>

struct Robot
{
    using Vector = std::pair<int, int>;
    int x = 0, y = 0, dx = 0, dy = 1;
    enum Color { Black = 0, White = 1 };
    std::map<Vector, Color> hull;
    Color get() { return hull[{x, y}]; }
    void turnLeft()
    {
        std::swap(dx, dy);
        dy = -dy;
        x += dx;
        y += dy;
    }
    void turnRight()
    {
        std::swap(dx, dy);
        dx = -dx;
        x += dx;
        y += dy;
    }
    void paint(bool white)
    {
        hull[{x, y}] = white ? Color::White : Color::Black;
    }

};

auto paint(Program program, bool white)
{
    Robot robot;
    robot.paint(white);

    while (!program.done())
    {
        program.push(robot.get());
        program.run();
        robot.paint(program.pop());
        if (program.pop() == 0)
            robot.turnLeft();
        else
            robot.turnRight();
    }
    return robot;
}

auto part1(const Program& program)
{
    auto robot = paint(program, false);
    return robot.hull.size();
}

auto part2(const Program& program)
{
    auto robot = paint(program, true);

    int x1 = std::numeric_limits<int>::max();
    int y1 = std::numeric_limits<int>::max();
    int x2 = std::numeric_limits<int>::min();
    int y2 = std::numeric_limits<int>::min();

    for (auto [pos, color] : robot.hull)
    {
        auto [x, y] = pos;
        x1 = std::min(x1, x);
        y1 = std::min(y1, y);
        x2 = std::max(x2, x);
        y2 = std::max(y2, y);
    }
    auto w = x2 - x1 + 1;
    auto h = y2 - y1 + 1;
    std::vector<std::string> lines(h, std::string(w, ' '));

    for (auto [pos, color] : robot.hull)
    {
        auto [x, y] = pos;
        if (color == 1)
        lines[h - (y-y1) - 1][w - (x-x1) - 1] = '#';
    }
    return join(lines, "\n");
}

int main()
{
    std::ifstream infile("data.txt");
    std::string source;
    std::getline(infile, source);
    auto program = Program { source };
    std::cout << part1(program) << std::endl; // 2293
    std::cout << part2(program) << std::endl; // AHLCPRAL
    return 0;
}
