#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

auto parse(auto infile)
{
    std::string result;
    int width = 0;
    for (std::string line;std::getline(infile,line);)
    {
        result += line;
        width = line.size();
    }
    return std::pair { result, width };
}

auto part1(auto input)
{
    auto [s, w ] = input;
    const auto n = s.size();

    for (int i = 1;;++i)
    {
        std::string t(n, '.');
        std::vector<bool> m(n, false);
        int count = 0;
        for (int j = 0;j < n;++j)
        {
            int x = (j % w);
            int k = j - x + ((x+1) % w);
            if (s[j] == '>' && s[k] == '.')
            {
                m[j] = true;
                ++count;
            }
        }
        for (int j = 0;j < n;++j)
            if (m[j])
            {
                int x = (j % w);
                int k = j - x + ((x+1) % w);
                s[k] = s[j];
                s[j] = '.';
            }
        m = std::vector<bool>(n, false);
        for (int j = 0;j < n;++j)
        {
            int k = (j + w) % n;
            if (s[j] == 'v' && s[k] == '.')
            {
                m[j] = true;
                ++count;
            }
        }
        for (int j = 0;j < n;++j)
            if (m[j])
            {
                s[(j+w) % n] = s[j];
                s[j] = '.';
            }

        if (count == 0) return i;
    }
    return 0;
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 474
    return 0;
}
