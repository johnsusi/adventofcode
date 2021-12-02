#include <fstream>
#include <iostream>
#include <string>

int main()
{
    std::ifstream infile("data.txt");

    std::string command;
    int value;

    long horizontal = 0, depth1 = 0, depth2 = 0, aim = 0;
    while (infile >> command >> value)
    {
        if (command == "up")
        {
            depth1 -= value;
            aim -= value;
        }
        else if (command == "down")
        {
            depth1 += value;
            aim += value;
        }
        else if (command == "forward")
        {
            horizontal += value;
            depth2 += aim * value;
        }
    }

    std::cout << horizontal * depth1 << std::endl;
    std::cout << horizontal * depth2 << std::endl;

    return 0;
}