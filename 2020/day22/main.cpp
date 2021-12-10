#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using deck_t = std::vector<int>;

int score(deck_t deck)
{
    int score = 0;
    for (int i = deck.size() - 1, j = 1; i >= 0; --i, ++j) score += deck[i] * j;
    return score;
}

std::string hash(const deck_t& deck)
{
    std::string result;
    bool first = true;
    for (auto card : deck) {
        if (!first) result += "_";
        result += std::to_string(card);
        first = false;
    }
    return result;
}

std::map<std::string, std::pair<int, deck_t>> played_before;
std::pair<int, deck_t> round(deck_t p1, deck_t p2, bool part2)
{
    std::set<std::string> visited;
    while (true) {
        auto [it, ok] = visited.insert(hash(p1) + ":" + hash(p2));
        if (!ok) return {1, p1};
        int c1 = p1.front(), c2 = p2.front();
        p1.erase(p1.begin());
        p2.erase(p2.begin());
        int winner = 0;

        if (part2 && c1 <= p1.size() && c2 <= p2.size()) {
            deck_t pp1;
            for (int i = 0; i < c1; ++i) pp1.push_back(p1[i]);
            deck_t pp2;
            for (int i = 0; i < c2; ++i) pp2.push_back(p2[i]);
            auto result = round(pp1, pp2, part2);
            winner      = result.first;
        }
        else if (c1 > c2)
            winner = 1;
        else
            winner = 2;

        if (winner == 1) {
            p1.push_back(c1);
            p1.push_back(c2);
        }
        else {
            p2.push_back(c2);
            p2.push_back(c1);
        }
        if (p2.empty()) return {1, p1};
        if (p1.empty()) return {2, p2};
    }
}

int part1(std::map<std::string, deck_t> players)
{
    auto [winner, deck] = round(players["Player 1"], players["Player 2"], false);
    return score(deck);
}

int part2(std::map<std::string, deck_t> players)
{
    auto [winner, deck] = round(players["Player 1"], players["Player 2"], true);
    return score(deck);
}

int main()
{
    std::ifstream infile("data.txt");
    std::map<std::string, deck_t> players;

    std::string player;
    for (std::string line; std::getline(infile, line);) {
        if (line.empty()) continue;
        if (line.starts_with("Player "))
            player = line.substr(0, line.size() - 1);
        else
            players[player].push_back(std::stoi(line));
    }

    std::cout << part1(players) << std::endl; // 33772
    std::cout << part2(players) << std::endl; // 35070
    return 0;
}
