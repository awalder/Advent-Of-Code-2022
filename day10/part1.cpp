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
    auto file = std::ifstream{"../day10/example.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    int registerValue = 1;
    // int cycle = 0;
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
    values.push_back(registerValue);

    int signalStrength = 0;
    for(size_t i = 0; i < values.size(); ++i)
    {
        if(i >= 20 && (i - 20) % 40 == 0)
        {
            auto cur = values.at(i - 1) * i;
            signalStrength += cur;
        }
    }

    fmt::print("final Strength: {}\n", signalStrength);
}
