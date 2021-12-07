#include <cassert>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include <common/string.hpp>

struct Chemical
{
    std::string name;
    std::int64_t quantity;
};

struct Reaction
{
    Chemical output;
    std::vector<Chemical> inputs;
};

auto cost(const std::map<std::string, Reaction>& reactions, std::int64_t amount)
{
    std::map<std::string, std::int64_t> amounts;
    std::deque< Chemical > fringe { { "FUEL", amount }};
    std::int64_t result = 0;
    while (!fringe.empty())
    {
        auto [name, quantity] = fringe.front();
        fringe.pop_front();
        if (amounts[name] > 0)
        {
            auto t = std::min(quantity, amounts[name]);
            quantity -= t;
            amounts[name] -= t;
        }
        if (quantity > 0)
        {
            auto formula = reactions.at(name);
            std::int64_t n = (std::int64_t)std::ceil(quantity / (double)formula.output.quantity);
            for (auto input : formula.inputs)
            {
                if (input.name == "ORE") result += n * input.quantity;
                else fringe.push_back({ input.name, n * input.quantity });
            }

            amounts[name] += formula.output.quantity * n - quantity;

        }
    }
    return result;
}

auto part1(const std::map<std::string, Reaction>& reactions)
{
    return cost(reactions, 1);
}

auto part2(const std::map<std::string, Reaction>& reactions)
{
    std::int64_t fuel = 1, step = 1;

    while (1000000000000ll > cost(reactions, fuel))
    {
        step *= 2;
        fuel += step;
    }
    // Works for this input but not very generic or elegant
    while (step >= 1)
    {
        step /= 2;
        if (1000000000000ll > cost(reactions, fuel))
            fuel += step;
        else
            fuel -= step;
    }
    while (1000000000000ll < cost(reactions, fuel))
        --fuel;

    return fuel;
}

int main()
{
    std::ifstream infile("data.txt");
    std::map<std::string, Reaction> reactions;
    for (std::string line; std::getline(infile, line);)
    {
        auto [inputs, output] = split(line, " => ");
        auto chemicals = split_all(inputs, ", ");
        auto [q, n] = split(output, " ");
        auto reaction = Reaction {
            Chemical { n, std::stoi(q) }
        };

        for (auto c : chemicals)
        {
            auto [q, n] = split(c, " ");
            reaction.inputs.push_back({ n, std::stoi(q)});
        }

        reactions[n] = reaction;
    }

    std::cout << part1(reactions) << std::endl; // 598038
    std::cout << part2(reactions) << std::endl; // 2269325
    return 0;
}
