#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto play(std::string source, int n, int moves)
{
    std::vector<int> cups(n+1);
    int cur = (int)(source[0] - '0'), last = (int)(source[source.size() - 1] - '0');

    for (int i = 1; i < source.size(); ++i) cups[source[i - 1] - '0'] = source[i] - '0';

    if (n > source.size()) {
        for (int i = source.size() + 1; i < n; ++i) cups[i] = i + 1;
        cups[last] = source.size() + 1;
        cups[n]    = cur;
    }
    else
        cups[last] = cur;

    int min = 1;
    int max = n;
    for (int i = 0; i < moves; ++i) {

        int a = cups[cur];
        int b = cups[a];
        int c = cups[b];
        int d = cups[c];

        int dest = cur - 1;
        if (dest < min) dest = max;
        while (dest == a || dest == b || dest == c)
            if (--dest < min) dest = max;

        cur = cups[cur] = d;
        cups[c]         = cups[dest];
        cups[dest]      = a;
    }
    return cups;
}

std::string part1(const std::string& input, int moves = 100)
{
    auto cups = play(input, input.size(), moves);
    std::string result;
    for (int i = 1, j = cups[1]; i < input.size(); ++i, j = cups[j]) result.push_back('0' + j);
    return result;
}

long part2(const std::string& input)
{
    auto cups = play(input, 1000000, 10000000);
    return (long)cups[1] * (long)cups[cups[1]];
}

int main()
{
    assert(part1("389125467", 10) == "92658374");
    assert(part1("389125467", 100) == "67384529");
    assert(part2("389125467") == 149245887792l);

    std::ifstream infile("data.txt");
    std::string line;
    std::getline(infile, line);
    std::cout << part1(line) << std::endl; // 49576328
    std::cout << part2(line) << std::endl; // 511780369955
    return 0;
}
