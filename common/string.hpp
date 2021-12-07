#pragma once
#include <cassert>
#include <numeric>
#include <string>
#include <vector>

std::string to_string(const std::string& value)
{
    return value;
}
template <typename C>
auto join(C&& container, std::string delim)
{
    using namespace std;;
    auto s = begin(container);
    auto e = end(container);
    if (s == e) return string{};
    auto init = to_string(*s);
    advance(s, 1);
    return accumulate(s, e, init,
        [&](const auto& a, auto b){
            return a + delim + to_string(b);
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
      result.push_back(s.substr(j, i-j));

  return result;
}

auto split(const std::string& s, const std::string& delimiter)
{
  int i = s.find(delimiter);
  assert(i != std::string::npos);
  return std::pair { s.substr(0, i), s.substr(i+delimiter.size(), s.size() - i - delimiter.size())};
}

auto repeat(const std::string& input, std::size_t num)
{
    std::string result;
    result.reserve(input.size() * num);
    while (num--)
        result += input;
    return result;
}