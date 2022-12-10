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
#include <benchmark/benchmark.h>

struct Tree
{
    int height = 0;
    int scenicScore = 0;
};

// Dimensions

using TreeGrid = std::vector<Tree>;

static void BM_Anssi_Day8(benchmark::State& state)
{
    auto file = std::ifstream{"../day8/input.txt", std::ios::in};
    std::vector<std::string> input = [&file] {
        std::vector<std::string> res;
        std::string line;
        while(std::getline(file, line))
        {
            res.emplace_back(line);
        }
        return res;
    }();
    file.close();

    size_t width = 0;
    size_t height = 0;

    for(auto _ : state)
    {
        TreeGrid grid;
        width = 0;
        height = 0;

        for(const auto& line : input)
        {
            width = line.length();
            height += 1;
            ranges::for_each(std::as_const(line), [&grid](char c) {
                grid.push_back({c - '0'});
            });
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
                                auto i = indexFunc(r);
                                auto& otherTree = grid.at(i);
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

                tree.scenicScore =
                        ranges::accumulate(scores, 1, std::multiplies{});
            }
        }

        auto result = std::max_element(
                grid.begin(), grid.end(), [](const Tree& lhs, const Tree& rhs) {
                    return lhs.scenicScore < rhs.scenicScore;
                });
        benchmark::DoNotOptimize(result);

        // verify result
        assert(259308 == result->scenicScore);
    }
}

BENCHMARK(BM_Anssi_Day8);
BENCHMARK_MAIN();
