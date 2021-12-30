#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

int main()
{
    std::ifstream infile("data.txt");

    int depart;
    infile >> depart;
    std::string schedule;
    std::getline(infile, schedule);
    std::getline(infile, schedule);

    std::vector<long> a;
    for (int i = 0; i < schedule.size();) {
        int j = schedule.find(',', i);
        if (j == std::string::npos) j = schedule.size() + 1;
        std::string x = schedule.substr(i, j - i);
        i             = j + 1;
        a.push_back(x == "x" ? 1 : std::stoi(x));
    }

    std::vector<std::pair<int, int>> result;
    for (auto id : a) {
        if (id == 1) continue;
        result.emplace_back(id - depart % id, (id - depart % id) * id);
    }

    auto x = std::min_element(
        result.begin(), result.end(), [](auto a, auto b) { return a.first < b.first; });
    std::cout << x->second << std::endl; // 138

    long timestamp = 0;
    for (long i = 1, step = a[0]; i < a.size(); ++i) {
        while ((timestamp + i) % a[i] != 0) timestamp += step;
        step *= a[i];
    }

    std::cout << timestamp << std::endl; // 226845233210288
    return 0;
}

