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

auto print(Knot& head, Knot& tail) -> void
{

    // dims x [0, 5]
    // dims y [0, 5]

    fmt::print("** New board ** \n");
    for(int j = 0; j < 5; ++j)
    {
        for(int i = 0; i < 5; ++i)
        {
            if(tail == Knot{i, j})
            {
                fmt::print("T");
            }
            else if(head == Knot{i, j})
            {
                fmt::print("H");
            }
            else
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

    Knot head;
    Knot tail;

    std::set<Knot> visitedPositions;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        char direction = 0;
        int distance = 0;
        ss >> direction >> distance;

        // fmt::print("Move ({}::{})\n", direction, distance);

        for(int i = 0; i < distance; ++i)
        {
            switch(direction)
            {
            case 'L': head.x -= 1;
            case 'R': head.x += 1;
            case 'U': head.y += 1;
            case 'D': head.y -= 1;
            }

            auto dx = head.x - tail.x;
            auto dy = head.y - tail.y;
            // fmt::print("dx {} : dy {}\n", dx, dy);
            if(std::abs(dx) + std::abs(dy) >= 3)
            {
                tail.x += std::clamp(dx, -1, 1);
                tail.y += std::clamp(dy, -1, 1);
            }
            else
            {
                if(std::abs(dx) > 1)
                {
                    tail.x += std::clamp(dx, -1, 1);
                }
                if(std::abs(dy) > 1)
                {
                    tail.y += std::clamp(dy, -1, 1);
                }
            }

            visitedPositions.emplace(tail.x, tail.y);
            // print(head, tail);
        }
    }

    auto result = visitedPositions.size();
    fmt::print("Result {}\n", result);
}
