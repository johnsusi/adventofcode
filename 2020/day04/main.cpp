#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <common/string.hpp>

bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

bool is_hex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}

bool is_valid_byr(std::string val)
{
    return val.size() == 4 && std::all_of(val.begin(), val.end(), is_digit) &&
           std::stoi(val) >= 1920 && std::stoi(val) <= 2002;
}

bool is_valid_iyr(std::string val)
{
    return val.size() == 4 && std::all_of(val.begin(), val.end(), is_digit) &&
           std::stoi(val) >= 2010 && std::stoi(val) <= 2020;
}

bool is_valid_eyr(std::string val)
{
    return val.size() == 4 && std::all_of(val.begin(), val.end(), is_digit) &&
           std::stoi(val) >= 2020 && std::stoi(val) <= 2030;
}

bool is_valid_hgt(std::string val)
{
    if (val.size() <= 2) return false;

    bool cm  = val[val.size() - 2] == 'c' && val[val.size() - 1] == 'm';
    bool in  = val[val.size() - 2] == 'i' && val[val.size() - 1] == 'n';
    auto num = val.substr(0, val.size() - 2);
    if (!std::all_of(num.begin(), num.end(), is_digit)) return false;
    auto h = std::stoi(num);
    if (cm) return h >= 150 && h <= 193;
    if (in) return h >= 59 && h < 76;
    return false;
}

bool is_valid_ecl(std::string val)
{
    return val == "amb" || val == "blu" || val == "brn" || val == "gry" || val == "grn" ||
           val == "hzl" || val == "oth";
}

bool is_valid_hcl(std::string val)
{
    return val.size() == 7 && val[0] == '#' && std::all_of(++val.begin(), val.end(), is_hex);
}

bool is_valid_pid(std::string val)
{
    return val.size() == 9 && std::all_of(val.begin(), val.end(), is_digit);
}

auto parse(auto&& infile)
{    
    std::map<std::string, std::string> passport;    
    std::vector<decltype(passport)> passports;
    
    for (std::string line; std::getline(infile, line);) {
        if (line.empty()) {
            if (!passport.empty()) {
                passports.push_back(passport);
                passport = {};
            }
            continue;
        }

        for (auto&& pair : split_all(line, " ")) {
            auto&& [k, v] = split(pair, ":");
            passport[k]   = v;
        }
    }
    if (!passport.empty()) passports.push_back(passport);
    return passports;
}

auto part1(auto input)
{
    return std::count_if(input.begin(), input.end(), [](auto passport)
    {
        const auto names = std::vector {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};
        return std::all_of(
            names.begin(), names.end(), [&](const auto& x) { return passport.contains(x); });
    });
}

auto part2(auto input)
{
   return std::count_if(input.begin(), input.end(), [](auto passport)
   {
       return is_valid_byr(passport["byr"]) &&
           is_valid_iyr(passport["iyr"]) && is_valid_eyr(passport["eyr"]) &&
           is_valid_hgt(passport["hgt"]) && is_valid_hcl(passport["hcl"]) &&
           is_valid_ecl(passport["ecl"]) && is_valid_pid(passport["pid"]);
   });
}

int main()
{
    auto input = parse(std::ifstream("data.txt"));
    std::cout << part1(input) << std::endl; // 235
    std::cout << part2(input) << std::endl; // 194
    return 0;
}
