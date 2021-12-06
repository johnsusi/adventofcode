#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

int main()
{

  std::ifstream infile("data.txt");

  int r1 = 0, r2 = 0;
  std::string line;
  std::regex test("([0-9]+)-([0-9]+) ([a-z]): ([a-z]+)");
  while (std::getline(infile, line))
  {
    std::smatch m;
    assert(std::regex_match(line, m, test));

    int lo = std::stoi(m[1].str());
    int hi = std::stoi(m[2].str());
    char c = m[3].str()[0];
    auto w = m[4].str();

    int count = std::count(w.begin(), w.end(), c);

    if (count >= lo && count <= hi) ++r1;

    auto m1 = lo <= w.size() && w.at(lo-1) == c;
    auto m2 = hi <= w.size() && w.at(hi-1) == c;

    if (m1 != m2)
      ++r2;
  }

  std::cout << r1 << std::endl; // 638
  std::cout << r2 << std::endl; // 699

  return 0;
}
