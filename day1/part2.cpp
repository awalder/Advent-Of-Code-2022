#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <algorithm>
#include <numeric>

int main()
{
    auto file = std::ifstream{"../day1/input1.txt", std::ios::in};

    std::vector<int> calories(1);
    std::size_t i = 0;

    if(file)
    {
        std::string line;
        while(std::getline(file, line))
        {
            if(line.empty())
            {
                calories.push_back(0);
                i += 1;
            }
            std::stringstream ss(line);

            int val = 0;
            ss >> val;

            calories[i] += val;
        }
    }

    std::sort(calories.rbegin(), calories.rend());
    auto sum = std::accumulate(calories.begin(), calories.begin() + 3, 0);
    fmt::print("Sum of Top 3 cals: {}\n", sum);
}
