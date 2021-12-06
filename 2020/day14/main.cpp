#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using mask_t = std::bitset<36>;

struct op_t {
    uint64_t address;
    uint64_t value;
    mask_t clr_mask, set_mask, flt_mask;
};

std::vector<op_t> parse(std::istream&& source)
{
    std::vector<op_t> program;
    mask_t clr_mask, set_mask, flt_mask;

    for (std::string line; std::getline(source, line);) {
        std::istringstream parser(line);
        std::string cmd;
        parser >> cmd;
        if (cmd == "mask") {
            std::string delim, mask;
            parser >> delim >> mask;
            clr_mask = set_mask = flt_mask = 0;
            for (auto bit : mask) {
                clr_mask <<= 1;
                set_mask <<= 1;
                flt_mask <<= 1;
                if (bit == '0')
                    clr_mask |= 1;
                else if (bit == '1')
                    set_mask |= 1;
                else if (bit == 'X')
                    flt_mask |= 1;
            }
        }
        else if (cmd.substr(0, 3) == "mem") {
            std::string delim;
            uint64_t value;
            parser >> delim >> value;
            auto arg      = cmd.substr(4, cmd.size() - 5);
            uint64_t addr = std::stoi(arg);

            program.push_back({addr, value, clr_mask, set_mask, flt_mask});
        }
    }
    return program;
}

uint64_t part1(const std::vector<op_t>& program)
{
    std::map<uint64_t, uint64_t> mem;
    for (auto op : program) {
        auto value      = (op.value & ~op.clr_mask.to_ulong()) | op.set_mask.to_ulong();
        mem[op.address] = value;
    }
    uint64_t result = 0;
    for (auto [addr, val] : mem) result += val;
    return result;
}

uint64_t part2(const std::vector<op_t>& program)
{
    std::map<uint64_t, uint64_t> mem;
    for (auto op : program) {
        int floating = 1UL << op.flt_mask.count();
        std::array<int, 36> index;
        index.fill(0);
        for (uint64_t i = 0, j = 0; i < 36; ++i)
            if (op.flt_mask[i]) index[j++] = i + 1;
        auto addr = op.address | op.set_mask.to_ulong();
        for (int i = 0; i < floating; ++i) {
            uint64_t a = addr & ~op.flt_mask.to_ulong();
            uint64_t m = 0;
            for (int j = 0; j < 36; ++j)
                if (index[j] > 0) m |= ((i & (1UL << j)) != 0 ? (1UL << (index[j] - 1)) : 0);
            a |= m;
            mem[a] = op.value;
        }
    }
    uint64_t result = 0;
    for (auto [addr, val] : mem) result += val;
    return result;
}

int main()
{
    auto program = parse(std::ifstream("data.txt"));
    std::cout << part1(program) << std::endl;
    std::cout << part2(program) << std::endl;
    return 0;
}