#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

int memory(std::vector<int> start, int turns)
{
    int a = start[0], n = start.size();
    std::vector<int> numbers(turns, 0);
    for (int i = 1; i < turns; ++i) {
        int b      = i < n ? start[i] : numbers[a] > 0 ? i - numbers[a] : 0;
        numbers[a] = i;
        a          = b;
    }
    return a;
}

int main()
{
    auto parser = std::ifstream("data.txt");
    std::vector<int> start;
    for (int a; parser >> a; parser.get()) start.push_back(a);

    assert(memory({0, 3, 6}, 2020) == 436);
    assert(memory({1, 3, 2}, 2020) == 1);
    assert(memory({2, 1, 3}, 2020) == 10);
    assert(memory({1, 2, 3}, 2020) == 27);
    assert(memory({2, 3, 1}, 2020) == 78);
    assert(memory({3, 2, 1}, 2020) == 438);
    assert(memory({3, 1, 2}, 2020) == 1836);

    std::cout << memory(start, 2020) << std::endl;     // 1618
    std::cout << memory(start, 30000000) << std::endl; // 548531
    return 0;
}
