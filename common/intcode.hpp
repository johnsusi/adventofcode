#pragma once

#include "string.hpp"

#include <deque>
#include <iostream>
#include <sstream>
#include <vector>

struct Program {
    std::vector<std::int64_t> opcodes;
    int ip = 0;
    int rb = 0;
    std::deque<std::int64_t> input, output;

    Program(const std::string& source)
    {
        std::stringstream parser(source);
        char delim;
        for (std::int64_t opcode; parser >> opcode; parser >> delim) opcodes.push_back(opcode);
    }

    std::string to_string() const { return join(opcodes, ","); }

    std::int64_t pop()
    {
        assert(!output.empty());
        std::int64_t result = output.front();
        output.pop_front();
        return result;
    }

    void push(std::int64_t value)
    {
        input.push_back(value);
    }

    std::int64_t& operator[](int index) { return opcodes[index]; }

    std::int64_t get(std::int64_t offset, int mode)
    {
        switch (mode) {
        case 0: // position mode
            assert(offset >= 0);
            if (opcodes.size() < offset+1)
                opcodes.resize(offset+1, 0);
            return opcodes[offset];
        case 1: // immediate mode
            return offset;
        case 2: // relative mode
            assert(offset + rb >= 0);
            if (opcodes.size() < offset + rb + 1)
                opcodes.resize(offset + rb + 1, 0);
            return opcodes[offset + rb];
        default: std::terminate();
        }
    }

    void set(int offset, int mode, std::int64_t value)
    {
        assert(mode != 1);
        switch (mode) {
        case 0: // position mode
            assert(offset >= 0);
            if (opcodes.size() < offset+1)
                opcodes.resize(offset+1, 0);
            opcodes[offset] = value;
            break;
        case 2: // relative mode
            assert(offset + rb >= 0);
            if (opcodes.size() < offset + rb + 1)
                opcodes.resize(offset + rb + 1, 0);
            opcodes[offset + rb] = value;
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
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax + bx);
                break;
            }
            case 2: // mul
            {
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
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
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
                if (ax != 0) ip = bx;
                break;
            }
            case 6: // jump-if-false
            {
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
                if (ax == 0) ip = bx;
                break;
            }
            case 7: // less
            {
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax < bx ? 1 : 0);
                break;
            }
            case 8: // equals
            {
                auto ax = get(opcodes[ip++], c);
                auto bx = get(opcodes[ip++], b);
                set(opcodes[ip++], a, ax == bx ? 1 : 0);
                break;
            }
            case 9: // equals
            {
                auto ax = get(opcodes[ip++], c);
                rb += ax;
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