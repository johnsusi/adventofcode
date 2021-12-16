#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

template <typename T> void intersect(std::set<T>& source, const std::set<T>& other)
{
    std::set<T> temp;
    std::set_intersection(source.begin(),
                          source.end(),
                          other.begin(),
                          other.end(),
                          std::inserter(temp, temp.begin()));
    source = temp;
}

struct food_t {
    std::set<std::string> ingredients;
    std::set<std::string> allergens;
};

std::pair<std::vector<food_t>, std::map<std::string, std::string>> parse(std::istream&& source)
{
    std::vector<food_t> foods;
    std::map<std::string, std::string> allergens;
    for (std::string line; std::getline(source, line);) {
        std::istringstream parser(line);
        std::string token;
        food_t food;
        while (parser >> token && token != "(contains") food.ingredients.insert(token);
        while (parser >> token) {
            if (token.ends_with(",") || token.ends_with(")"))
                token = token.substr(0, token.size() - 1);
            food.allergens.insert(token);
            allergens[token] = "";
        }
        foods.push_back(food);
    }
    return {foods, allergens};
}

void filter(std::vector<food_t>& foods, std::map<std::string, std::string>& allergens)
{
    std::set<std::string> all;
    for (const auto& food : foods)
        for (const auto& ingredient : food.ingredients) all.insert(ingredient);

    int count;
    do {
        count = 0;
        for (auto& [k, v] : allergens) {
            if (!v.empty()) continue;
            std::set<std::string> matches = all;
            for (const auto& food : foods)
                if (food.allergens.contains(k)) intersect(matches, food.ingredients);
            if (matches.size() != 1) continue;
            v = *matches.begin();
            for (auto& food : foods) {
                food.allergens.erase(k);
                food.ingredients.erase(v);
            }
            ++count;
        }
    } while (count > 0);
}

auto part1(const std::vector<food_t>& foods)
{
    int result = 0;
    for (const auto& food : foods) result += food.ingredients.size();
    return result;
}

auto part2(const std::map<std::string, std::string>& allergens)
{
    std::string result;
    for (auto&& [k, v] : allergens)
        result +=
            result.empty() ? v : "," + v;
    return result;
}

int main()
{
    auto [foods, allergens] = parse(std::ifstream("data.txt"));
    filter(foods, allergens);
    std::cout << part1(foods) << std::endl; // 2020
    std::cout << part2(allergens) << std::endl;  // bcdgf,xhrdsl,vndrb,dhbxtb,lbnmsr,scxxn,bvcrrfbr,xcgtv
    return 0;
}
