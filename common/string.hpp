#pragma once
#include <cassert>
#include <numeric>
#include <string>
#include <vector>

template <typename C>
auto join(C&& container, std::string delim)
{
    auto begin = std::begin(container);
    auto end = std::end(container);
    if (begin == end) return std::string{};
    auto init = std::to_string(*begin);
    std::advance(begin, 1);
    return std::accumulate(begin, end, init,
        [&](const auto& a, auto b){
            return a + delim + std::to_string(b);
        });
}

auto split_all(const std::string& s, const std::string& delimiter)
{
  std::vector<std::string> result;
  for (int i = s.find(delimiter), j = 0;;j = i+delimiter.size(), i = s.find(delimiter, j))
    if (i == std::string::npos)
    {
      result.push_back(s.substr(j, s.size() - j));
      break;
    }
    else
      result.push_back(s.substr(j, i-j+1-delimiter.size()));

  return result;
}

auto split(const std::string& s, const std::string& delimiter)
{
  int i = s.find(delimiter);
  assert(i != std::string::npos);
  return std::pair { s.substr(0, i), s.substr(i+delimiter.size(), s.size() - i - delimiter.size())};
}