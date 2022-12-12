#include <charconv>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <algorithm>
#include <array>
#include <string_view>
#include <memory>
#include <tuple>
#include <numeric>
#include <cmath>
#include <range/v3/all.hpp>
#include <fmt/ranges.h>
#include <functional>
#include <set>

int main()
{
    auto file = std::ifstream{"../day10/input.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    int registerValue = 1;
    std::vector<int> values;

    while(std::getline(file, line))
    {
        if(line.starts_with("noop"))
        {
            values.push_back(registerValue);
        }
        else if(line.starts_with("addx"))
        {
            int value = 0;
            auto start = line.data() + 5;
            auto end = line.data() + line.size();
            std::from_chars(start, end, value);
            values.push_back(registerValue);
            values.push_back(registerValue);
            registerValue += value;
        }
    }

    std::string row(40, '.');

    for(size_t i = 0; i < values.size(); ++i)
    {
        const auto cycle = i + 1;
        const auto spritePosition = values.at(i);
        const auto pixelIndex = static_cast<int>(i % 40);
        auto& pixel = row.at(pixelIndex);
        if(spritePosition - 1 <= pixelIndex && pixelIndex <= spritePosition + 1)
        {
            pixel = '#';
        }
        if((cycle) % 40 == 0)
        {
            fmt::print("{}\n", row);
            ranges::fill(row, '.');
        }
    }
}
