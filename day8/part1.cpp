#include <fstream>
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

struct Tree
{
    int height = 0;
    bool visible = false;
};

// Dimensions
size_t width = 0, height = 0;

using TreeGrid = std::vector<Tree>;

auto print(const TreeGrid& grid) -> void
{
    fmt::print("** Start print\n");
    for(size_t y = 0; y < height; ++y)
    {
        for(size_t x = 0; x < width; ++x)
        {
            size_t index = y * width + x;
            const auto& tree = grid.at(index);
            fmt::print("{} ", static_cast<int>(tree.visible));
        }
        fmt::print("\n");
    }
}

int main()
{
    auto file = std::ifstream{"../day8/input.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    TreeGrid grid;
    std::string line;

    while(std::getline(file, line))
    {
        width = line.length();
        height += 1;

        for(char c : line)
        {
            grid.push_back({c - '0', false});
        }
    }

    auto lookfrom =
            [&grid](int a, int b, int c, int d, bool vertical, auto indexFunc) {
                auto generateRange = [](int n, int m) {
                    auto range = std::vector<int>(std::abs(n - m));
                    std::iota(range.begin(), range.end(), std::min(n, m));
                    if(m < n)
                        std::reverse(range.begin(), range.end());
                    return range;
                };

                auto rangeX = generateRange(a, b);
                auto rangeY = generateRange(c, d);

                if(vertical)
                    std::swap(rangeX, rangeY);

                for(auto y : rangeY)
                {
                    int highest = -1;
                    for(auto x : rangeX)
                    {
                        size_t index = indexFunc(x, y);
                        auto& tree = grid.at(index);

                        if(highest < tree.height)
                        {
                            tree.visible = true;
                        }
                        highest = std::max(highest, tree.height);
                    }
                }
            };

    auto vertical = [width = width](
                            int a, int b) -> size_t { return a * width + b; };

    auto horizontal = [height = height](int a, int b) -> size_t {
        return b * height + a;
    };

    // approach from left
    lookfrom(0, width, 0, height, false, horizontal);
    // print(grid);

    // approach from right
    lookfrom(width, 0, 0, height, false, horizontal);
    // print(grid);

    // approach from top
    lookfrom(0, width, 0, height, true, vertical);
    // print(grid);

    // approach from bottom
    lookfrom(0, width, height, 0, true, vertical);
    // print(grid);

    auto result = ranges::count_if(grid, [](Tree& tree) { return tree.visible; });
    fmt::print("Result {}\n", result);
}
