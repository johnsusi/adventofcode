#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Machine {

    std::vector<std::string> instr;
    std::vector<int> args;
    std::vector<int> count;

    int ip;
    int ax;
    int swp;

    Machine(const std::string& source)
    {
        std::istringstream parser(source);
        std::string cmd;
        int arg;
        while (parser >> cmd >> arg) {
            instr.push_back(cmd);
            args.push_back(arg);
            count.push_back(0);
        }
    }

    void clear()
    {
        std::fill(count.begin(), count.end(), 0);
        ip = ax = 0;
    }

    void swap(std::string& op)
    {
        if (op == "nop")
            op = "jmp";
        else if (op == "jmp")
            op = "nop";
    }

    void swap_next()
    {
        if (swp >= 0) swap(instr[swp]);
        while (instr[++swp] == "acc")
            ;
        swap(instr[swp]);
    }

    int execute(bool exitOnInfiniteLoop)
    {
        clear();
        swp = -1;

        while (ip < instr.size()) {
            if (count[ip] > 0) {
                if (exitOnInfiniteLoop) return ax;
                clear();
                swap_next();
            }
            count[ip] += 1;
            if (auto op = instr[ip]; op == "nop")
                ++ip;
            else if (op == "acc")
                ax += args[ip++];
            else if (op == "jmp")
                ip += args[ip];
        }
        return ax;
    }
};

int main()
{
    auto infile  = std::ifstream("data.txt");
    auto source  = std::string(std::istreambuf_iterator<char>(infile), {});
    auto machine = Machine(source);

    std::cout << machine.execute(true) << std::endl;  // 1384
    std::cout << machine.execute(false) << std::endl; // 761

    return 0;
}
