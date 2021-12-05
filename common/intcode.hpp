#pragma once

#include "string.hpp"

#include <deque>
#include <iostream>
#include <sstream>
#include <vector>

struct Program {
    std::vector<long> opcodes;
    int ip = 0;
    std::deque<long> input, output;

    Program(const std::string& source)
    {
        std::stringstream parser(source);
        char delim;
        for (long opcode; parser >> opcode; parser >> delim) opcodes.push_back(opcode);
    }

    std::string to_string() const { return join(opcodes, ","); }

    long pop()
    {
        assert(!output.empty());
        long result = output.front();
        output.pop_front();
        return result;
    }

    void push(long value)
    {
        input.push_back(value);
    }

    long& operator[](int index) { return opcodes[index]; }

    long get(int offset, int mode)
    {
        switch (mode) {
        case 0: // position mode
            return opcodes[offset];
            break;
        case 1: // immediate mode
            return offset;
        default: std::terminate();
        }
    }

    void set(int offset, int mode, int value)
    {
        assert(mode == 0);
        switch (mode) {
        case 0: // position mode
            opcodes[offset] = value;
            break;
        default: std::terminate();
        }
    }

    bool done() const { return opcodes[ip] == 99; }

    bool run()
    {

        while (ip < opcodes.size()) {
            int op = opcodes[ip] % 100;
            int c  = (opcodes[ip] / 100) % 10;
            int b  = (opcodes[ip] / 1000) % 10;
            int a  = (opcodes[ip] / 10000) % 10;
            ++ip;
            switch (op) {
            case 1: // add
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax + bx);
                break;
            }
            case 2: // mul
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax * bx);
                break;
            }
            case 3: // store
            {
                if (input.empty()) { --ip; return false; }
                set(opcodes[ip++], c, input.front());
                input.pop_front();
                break;
            }
            case 4: // store
            {
                output.push_back(get(opcodes[ip++], c));
                break;
            }
            case 5: // jump-if-true
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                if (ax != 0) ip = bx;
                break;
            }
            case 6: // jump-if-false
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                if (ax == 0) ip = bx;
                break;
            }
            case 7: // less
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax < bx ? 1 : 0);
                break;
            }
            case 8: // equals
            {
                long ax = get(opcodes[ip++], c);
                long bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax == bx ? 1 : 0);
                break;
            }
            case 99: // return
                --ip;
                return true;
            default:
                std::cout << "Unknown opcode " << opcodes[ip - 1] << " at " << ip - 1 << std::endl;
                return true;
            }
        }
        std::terminate();
    }
};