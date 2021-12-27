#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile)
{
    auto input = std::string(7, '.');
    for (std::string line; std::getline(infile, line);)
    {
        for (auto ch : line)
            if (ch >= 'A' && ch <= 'D')
                input.push_back(ch);
    }
    return input;
}

template <typename T> auto make_priority_queue(T initial)
{
    auto queue = std::priority_queue<T, std::vector<T>, std::greater<T>>{};
    queue.push(initial);
    return queue;
}

int cost(char type)
{
    if (type == 'A')
        return 1;
    if (type == 'B')
        return 10;
    if (type == 'C')
        return 100;
    if (type == 'D')
        return 1000;
    std::terminate();
}

bool is_solved(const auto &state)
{
    return state[7] == '#' && state[8] == '#' && state[9] == '#' && state[10] == '#';
}

auto try_move(const auto& state, int from) -> std::pair<int,int>
{
    int steps = 0;
    int t1 = 1 + state[from] - 'A';
    int t2 = 2 + state[from] - 'A';
    const int n = state.size();
    int j = from;
    for (;j < t1;steps += (j == 0 ? 1 : 2), ++j) if (state[j+1] != '.') return {0, 0};
    for (;j > t2;steps += (j == 6 ? 1 : 2), --j) if (state[j-1] != '.') return {0, 0};
    j = 7 + state[from] - 'A';
    steps += 2;
    for (;j < n && state[j] == '.';++steps, j+=4)
        if (j+4 >= n) return {steps,j};
        else if (state[j+4] == '#') return {steps, j};
        else if (state[j+4] != '.') return {0, 0};
    return { 0,0};
}

auto enque(auto cost, auto state, auto &queue, int from, int to, int steps)
{
    state[to] = state[from];
    state[from] = '.';
    cost += steps * ::cost(state[to]);
    while (true)
    {
        bool done = true;
        for (int i = 0;i < 7;++i)
        {
            if (state[i] == '.') continue;
            auto [steps, to] = try_move(state, i);
            if (steps > 0)
            {
                if (to <= 6) std::terminate();
                cost += ::cost(state[i])*steps;
                state[i] = '.';
                state[to] = '#';
                done = false;
            }
        }
        if (done) break;
    }
    queue.emplace(cost, state);
}

auto solve(const auto& input)
{
    const auto n = input.size();
    auto fringe = make_priority_queue(std::tuple{0ull, input});
    while (!fringe.empty())
    {
        auto [cost, state] = fringe.top();
        fringe.pop();

        if (is_solved(state)) return cost;

        for (int i = 3; i >= 0; --i)
            for (int j = 7 + i, s1 = 0; j < n; j += 4, ++s1)
                if (state[j] == '.')
                    continue;
                else if (state[j] == '#')
                    break;
                else
                {
                    for (int k = 1 + i, s2 = s1 + 2; k >= 0 && state[k] == '.'; s2 += ((--k) == 0) ? 1 : 2)
                        enque(cost, state, fringe, j, k, s2);
                    for (int k = 2 + i, s2 = s1 + 2; k < 7 && state[k] == '.'; s2 += ((++k) == 6) ? 1 : 2)
                        enque(cost, state, fringe, j, k, s2);
                    break;
                }
    }
    return 0ull;
}

auto part1(auto input)
{
    return solve(input);
}

auto part2(auto input)
{
    input.insert(11, "DCBADBAC");
    return solve(input);
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 10526
    std::cout << part2(input) << std::endl; // 41284
    return 0;
}
