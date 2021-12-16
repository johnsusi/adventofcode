#include <fstream>
#include <iostream>
#include <string>

auto search(long lo, long hi)
{
    int result1 = 0, result2 = 0;
    for (long i = lo;i <= hi;++i)
    {
        auto s = std::to_string(i);
        if (s.size() != 6) continue;
        int con = 1;
        bool adj = false;
        bool adj2 = false;
        bool dec = false;
        for (int j = 1;j < 6;++j)
        {
            if (s[j-1] == s[j]) { adj = true; ++con; }
            else
            {
                if (con == 2) adj2 = true;
                con = 1;
            }
            if (s[j-1] > s[j]) dec = true;
        }
        if (con == 2) adj2 = true;
        if (adj  && !dec) ++result1;
        if (adj2 && !dec) ++result2;
    }
    return std::pair { result1, result2 };
}

int main()
{
    std::ifstream infile("data.txt");
    long lo, hi;
    char delim;
    infile >> lo >> delim >> hi;

    auto [part1, part2] = search(lo, hi);

    std::cout << part1 << std::endl; // 1079
    std::cout << part2 << std::endl; // 699

    return 0;
}
