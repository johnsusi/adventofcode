#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Board {
    std::vector<int> numbers;
    bool done = false;
};

long score(Board board, int last)
{
    int result = 0;
    for (auto x : board.numbers) result += x;
    return result * last;
}

bool mark(Board& board, int next)
{
    for (int i = 0; i < 25; ++i)
        if (board.numbers[i] == next) board.numbers[i] = 0;
    for (int i = 0; i < 5; ++i) {
        int row = 0, col = 0;
        for (int j = 0; j < 5; ++j) {
            row += board.numbers[i * 5 + j];
            col += board.numbers[i + j * 5];
        }
        if (row == 0 || col == 0) {
            board.done = true;
            return true;
        }
    }
    return false;
}

long part1(std::vector<Board> boards, std::vector<int> numbers)
{
    for (auto next : numbers) {
        for (auto& board : boards) {
            if (mark(board, next)) return score(board, next);
        }
    }
    return 0;
}

long part2(std::vector<Board> boards, std::vector<int> numbers)
{
    int left = boards.size();
    for (auto next : numbers) {
        for (auto& board : boards) {
            if (board.done) continue;
            if (mark(board, next) && --left == 0)
                return score(board, next);
        }
    }
    return 0;
}

int main()
{
    std::ifstream infile("data.txt");

    std::vector<int> numbers;
    std::string line;
    std::getline(infile, line);
    std::stringstream str(line);

    int next = 0;
    while (str >> next) {
        numbers.push_back(next);
        char delim;
        str >> delim;
        if (delim != ',') break;
    }

    std::vector<Board> boards;

    while (true) {

        int x;
        if (!(infile >> x)) break;
        Board board;
        board.numbers.push_back(x);
        for (int i = 1; i < 25; ++i) {
            if (!(infile >> x)) break;
            board.numbers.push_back(x);
        }

        boards.push_back(board);
    }

    std::cout << part1(boards, numbers) << std::endl; // 8136
    std::cout << part2(boards, numbers) << std::endl; // 12738

    return 0;
}