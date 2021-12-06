#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

using range_t = std::pair<int, int>;
using field_t = std::vector<range_t>;

std::vector<int> parse_ticket(const std::string& source)
{
    std::vector<int> ticket;
    std::istringstream parser(source);
    for (int code; parser >> code; parser.get()) ticket.push_back(code);
    return ticket;
}

std::function<bool(const range_t&)> in_range(int value)
{
    return [=](const range_t& range) { return value >= range.first && value <= range.second; };
}

int main()
{

    auto infile = std::ifstream("data.txt");
    std::map<std::string, field_t> fields;
    for (std::string line; std::getline(infile, line);) {
        if (line.empty()) break;

        const std::regex pattern("([a-zA-Z ]+): ([0-9]+)-([0-9]+) or ([0-9]+)-([0-9]+)");
        std::smatch m;

        if (!std::regex_match(line, m, pattern)) std::terminate();
        fields[m[1]] = {{std::atoi(m[2].str().c_str()), std::atoi(m[3].str().c_str())},
                        {std::atoi(m[4].str().c_str()), std::atoi(m[5].str().c_str())}};
    }
    std::string line;
    std::getline(infile, line);
    assert("your ticket:" == line);

    std::getline(infile, line);
    auto ticket = parse_ticket(line);
    std::getline(infile, line);
    std::getline(infile, line);
    assert("nearby tickets:" == line);

    std::vector<std::vector<int>> nearby_tickets;
    while (std::getline(infile, line)) nearby_tickets.push_back(parse_ticket(line));

    std::vector<int> invalid;

    auto is_valid = [&](int value) {
        for (auto [k, v] : fields)
            if (std::any_of(v.begin(), v.end(), in_range(value))) return true;
        return false;
    };

    std::vector<std::vector<int>> valid_nearby_tickets;
    for (auto nearby_ticket : nearby_tickets) {
        bool valid = true;
        for (auto value : nearby_ticket) {
          if (!is_valid(value)) {
                invalid.push_back(value);
                valid = false;
            }
        }
        if (valid) valid_nearby_tickets.push_back(nearby_ticket);
    }
    std::cout << std::accumulate(invalid.begin(), invalid.end(), 0) << std::endl;

    std::map<std::string, int> actual;

    auto find_fields = [&](int value, std::vector<std::string> reject = {}) {
        std::vector<std::string> result;
        auto in_range = [&](auto&& range) { return value >= range.first && value <= range.second; };
        for (auto [k, v] : fields)
            if (std::any_of(v.begin(), v.end(), in_range) && !actual.contains(k))
                result.push_back(k);
        std::sort(result.begin(), result.end());
        return result;
    };
    std::vector<bool> done(ticket.size(), false);

    for (int k = 0; k < ticket.size(); ++k) {
        for (int j = 0; j < ticket.size(); ++j) {
            if (done[j]) continue;
            auto it         = valid_nearby_tickets.begin();
            auto candidates = find_fields(it->at(j));

            for (; it != valid_nearby_tickets.end(); ++it) {
                auto x = find_fields(it->at(j));
                std::vector<std::string> c;
                std::set_intersection(candidates.begin(),
                                      candidates.end(),
                                      x.begin(),
                                      x.end(),
                                      std::back_inserter(c));
                candidates = c;
            }
            if (candidates.empty() || candidates.size() > 1) continue;
            actual[candidates[0]] = ticket[j];
            done[j]               = true;
            break;
        }
    }

    long sum = 1;
    for (auto [k, v] : actual) {
        if (k.starts_with("departure")) sum *= v;
    }

    std::cout << sum << std::endl;

    return 0;
}
