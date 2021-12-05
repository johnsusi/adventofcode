#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <common/string.hpp>

struct Image {
    std::size_t wide, tall, size;
    std::vector<std::string> layers;

    Image(std::string image, std::size_t wide, std::size_t tall)
        : wide(wide), tall(tall), size(wide * tall)
    {
        layers = std::vector{image.size() / size, std::string(size, ' ')};
        int i = 0, j = 0;
        for (auto c : image) {
            layers[i][j] = c;
            if (++j >= size) {
                j = 0;
                ++i;
            }
        }
    }
};

auto part1(const Image& image)
{
    int result = 0;
    int best   = image.size + 1;
    for (auto layer : image.layers) {
        auto zeros = std::count(layer.begin(), layer.end(), '0');
        if (zeros < best) {
            best   = zeros;
            result = std::count(layer.begin(), layer.end(), '1') *
                     std::count(layer.begin(), layer.end(), '2');
        }
    }

    return result;
}

auto part2(const Image& image)
{
    auto result = std::vector{image.tall, std::string(image.wide, '.')};

    for (auto layer : image.layers)
        for (int i = 0; i < image.size; ++i) {
            char& elem = result[i / image.wide][i % image.wide];
            if (elem == '.' && layer[i] != '2') elem = layer[i] == '1' ? '#' : ' ';
        }

    return join(result, "\n");
}

int main()
{
    std::ifstream infile("data.txt");
    std::string raw;
    std::getline(infile, raw);
    auto image = Image{raw, 25, 6};
    std::cout << part1(image) << std::endl; // 1935
    std::cout << part2(image) << std::endl; // CFLUL
    return 0;
}
