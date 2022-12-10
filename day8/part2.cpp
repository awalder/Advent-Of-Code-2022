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

struct Tree
{
    int height = 0;
    int scenicScore = 0;
};

// Dimensions
size_t width = 0, height = 0;

using TreeGrid = std::vector<Tree>;

auto print(const TreeGrid& grid) -> void
{
    fmt::print("** Start print **\n");
    for(size_t y = 0; y < height; ++y)
    {
        for(size_t x = 0; x < width; ++x)
        {
            size_t index = y * width + x;
            const auto& tree = grid.at(index);
            fmt::print("{} ", tree.scenicScore);
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
        ranges::for_each(std::as_const(line), [&grid](char c) { grid.push_back({c - '0'}); });
    }

    namespace rv = ranges::views;
    // No need to traverse edges
    auto vy = rv::iota(std::min(height, 1ul), height - 1ul);
    auto vx = rv::iota(std::min(width, 1ul), width - 1ul);

    std::array<int, 4> scores;
    for(const auto y : vy)
    {
        auto up = rv::iota(0u, y) | rv::reverse;
        auto down = rv::iota(y + 1, height);
        for(const auto x : vx)
        {
            auto& tree = grid.at(y * width + x);

            auto left = rv::iota(0u, x) | rv::reverse;
            auto right = rv::iota(x + 1, width);

            scores.fill(0);

            auto traverse =
                    [&](auto&& range, auto&& indexFunc, size_t scoreIndex) {
                        for(auto r : range)
                        {
                            auto& otherTree = grid.at(indexFunc(r));
                            scores.at(scoreIndex) += 1;
                            if(otherTree.height >= tree.height)
                            {
                                break;
                            }
                        }
                    };

            // Indexing tactics
            auto vertical = [&](auto i) { return y * width + i; };
            auto horizontal = [&](auto i) { return i * width + x; };

            traverse(left, vertical, 0);
            traverse(right, vertical, 1);
            traverse(up, horizontal, 2);
            traverse(down, horizontal, 3);

            tree.scenicScore = ranges::accumulate(scores, 1, std::multiplies{});
        }
    }

    auto result = std::max_element(
            grid.begin(), grid.end(), [](const Tree& lhs, const Tree& rhs) {
                return lhs.scenicScore < rhs.scenicScore;
            });
    fmt::print("Result {}\n", result->scenicScore);
    // auto max = ranges::max_element(grid, [](const Tree& tree) {return
    // tree.scenicScore});

    // auto lookto =
    //         [&grid](int a, int b, int c, int d, bool vertical, auto
    //         indexFunc) {
    //             auto generateRange = [](int n, int m) {
    //                 auto range = std::vector<int>(std::abs(n - m));
    //                 std::iota(range.begin(), range.end(), std::min(n, m));
    //                 if(m < n)
    //                     std::reverse(range.begin(), range.end());
    //                 return range;
    //             };
    //
    //             auto rangeX = generateRange(a, b);
    //             auto rangeY = generateRange(c, d);
    //
    //             if(vertical)
    //                 std::swap(rangeX, rangeY);
    //
    //             int highest = -1;
    //             for(auto y : rangeY)
    //             {
    //                 for(auto x : rangeX)
    //                 {
    //                     size_t index = indexFunc(x, y);
    //                     auto& tree = grid.at(index);
    //
    //                     if(highest < tree.height)
    //                     {
    //                         tree.visible = true;
    //                     }
    //                     highest = std::max(highest, tree.height);
    //                 }
    //             }
    //         };

    // auto calculateScenicScore = [&grid, width = width, height = height](
    //                                     int x, int y) {
    // auto generateRange = [](int n, int m) {
    //     auto range = std::vector<int>(std::abs(n - m));
    //     std::iota(range.begin(), range.end(), std::min(n, m));
    //     if(m < n)
    //         std::reverse(range.begin(), range.end());
    //     return range;
    // };
    //
    // auto getTreeHeight = [&grid, &width, &height](int x, int y) {
    //     const auto index = y * width + x;
    //     const auto& tree = grid.at(index);
    //     return tree.height;
    // };

    // const auto rangeX1 = generateRange(x, 0); // to left
    // const auto rangeX2 = generateRange(x, width);  // to right
    // const auto rangeY1 = generateRange(y, 0);      // to up
    // const auto rangeY2 = generateRange(y, height); // to down
    // const auto index = y * width + x;
    // const auto& tree = grid.at(index);

    // for(int xx = 0;;)
    // {
    // }

    // auto rangeX = generateRange(a, b);
    // auto rangeY = generateRange(c, d);
    // };

    // auto v = {4};
    // auto rrng = ranges::views::for_each(v, [](int i) {
    //     return ranges::views::linear_distribute(0, i, i)  ;
    // });
    //
    // fmt::print("RRNG : {}\n", rrng);

    // auto vertical = [width = width](
    //                         int a, int b) -> size_t { return a * width + b; };

    // auto horizontal = [height = height](int a, int b) -> size_t {
    //     return b * height + a;
    // };

    // auto generateRange = [](int n, int m) {
    //     auto range = std::vector<int>(std::abs(n - m));
    //     std::iota(range.begin(), range.end(), std::min(n, m));
    //     if(m < n)
    //         std::reverse(range.begin(), range.end());
    //     return range;
    // };

    // auto vi = ranges::views::for_each(ranges::views::ints(1, 6), [](int i) {
    //     return ranges::yield_from(ranges::views::repeat_n(i, i));
    // }) | ranges::to_vector;
    // std::cout << ranges::views::all(vi) << std::endl;

    // namespace ra = ranges::actions;
    // namespace rv = ranges::views;
    //
    // const auto vi = rv::iota(1, 9) | ranges::to_vector;
    // fmt::print("{}\n", vi);
    //
    // auto rng = vi | rv::remove_if([](int i) { return i % 2; })
    //            | rv::transform([](int i) { return std::to_string(i); });
    // fmt::print("{}\n", rng);

    // auto rb = rv::all(rng);
    // std::cout << rb << std::endl;
    // auto va = vi | ra::sort | ra::unique;

    // for(auto n : ranges::iota_view(50, 100) | rv::reverse
    //                      | rv::transform([](int i) { return i; }))
    // {
    //     fmt::print("{} ", n);
    // }
    // fmt::print("\n");
    //
    // int sum = ranges::accumulate(
    //         rv::ints(1, ranges::unreachable)
    //                 | rv::transform([](int i) { return i * i; }) |
    //                 rv::take(3),
    //         0);
    // fmt::print("Sum {}\n", sum);
    //
    // for(auto& v : vi)
    // {
    //     fmt::print("{} ", v);
    // }
    // fmt::print("\n");
    // print(grid);
    //
    // int result = 0;
    // ranges::for_each(
    //         grid, [&result](Tree& tree) { result += tree.scenicScore; });
    // fmt::print("Result {}\n", result);
}
