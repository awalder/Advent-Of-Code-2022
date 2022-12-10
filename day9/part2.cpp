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

struct Knot
{
    Knot() = default;
    Knot(int x, int y) : x(x), y(y) {}
    int x = 0;
    int y = 0;

    friend bool operator==(const Knot& lhs, const Knot& rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    friend bool operator<(const Knot& lhs, const Knot& rhs)
    {
        if(lhs.x != rhs.x)
            return lhs.x < rhs.x;
        return lhs.y < rhs.y;
    }
};

using Rope = std::vector<Knot>;

auto print(const Rope& rope) -> void
{
    // dims x [0, 5]
    // dims y [0, 5]

    fmt::print("** New board ** \n");
    for(int y = -15; y < 15; ++y)
    {
        for(int x = -15; x < 15; ++x)
        {
            bool found = false;
            for(size_t i = 0; i < rope.size(); ++i)
            {
                const auto& knot = rope.at(i);
                if(knot == Knot{x, y})
                {
                    if(i == 0)
                    {
                        fmt::print("H");
                    }
                    else
                    {
                        fmt::print("{}", i);
                    }
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                fmt::print(".");
            }
        }
        fmt::print("\n");
    }
}

int main()
{
    auto file = std::ifstream{"../day9/input.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;

    Rope rope(10);
    Knot& head = rope.front();
    Knot& tail = rope.back();

    std::set<Knot> visitedPositions;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        char direction = 0;
        int distance = 0;
        ss >> direction >> distance;

        for(int i = 0; i < distance; ++i)
        {
            // switch(direction)
            // {
            // case 'L': head.x -= 1;
            // case 'R': head.x += 1;
            // case 'U': head.y += 1;
            // case 'D': head.y -= 1;
            // default: assert(false);
            // }
            if(direction == 'L')
            {
                head.x -= 1;
            }
            else if(direction == 'R')
            {
                head.x += 1;
            }
            else if(direction == 'U')
            {
                head.y += 1;
            }
            else if(direction == 'D')
            {
                head.y -= 1;
            }

            for(size_t i = 1; i < rope.size(); ++i)
            {
                auto& p1 = rope.at(i - 1u);
                auto& p2 = rope.at(i);

                auto dx = p1.x - p2.x;
                auto dy = p1.y - p2.y;

                if(std::abs(dx) + std::abs(dy) >= 3)
                {
                    p2.x += std::clamp(dx, -1, 1);
                    p2.y += std::clamp(dy, -1, 1);
                }
                else
                {
                    if(std::abs(dx) > 1)
                    {
                        p2.x += std::clamp(dx, -1, 1);
                    }
                    if(std::abs(dy) > 1)
                    {
                        p2.y += std::clamp(dy, -1, 1);
                    }
                }
            }

            visitedPositions.emplace(tail.x, tail.y);
        }
    }

    auto result = visitedPositions.size();
    fmt::print("Result {}\n", result);
}
