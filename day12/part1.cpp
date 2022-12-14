#include <charconv>
#include <cmath>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <range/v3/all.hpp>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <optional>

struct MapNode
{
    int height = 0;
    int distance = -1;
    size_t prev = std::numeric_limits<size_t>::max();

    friend bool operator<(const MapNode& lhs, const MapNode& rhs)
    {
        return lhs.distance < rhs.distance;
    }
};

using Map = std::vector<MapNode>;
size_t width = 0;
size_t height = 0;

// namespace rg = ranges;
// namespace rv = ranges::views;

auto print(const Map& map)
{
    fmt::print("****** PRINT MAP *******\n");
    for(size_t i = 0; i < map.size(); ++i)
    {
        fmt::print("{:02} ", map.at(i).height);
        if(i > 0 && (i + 1) % width == 0)
            fmt::print("\n");
    }
    fmt::print("\n");

    fmt::print("****** PRINT INDICES *******\n");
    for(size_t i = 0; i < map.size(); ++i)
    {
        fmt::print("{:02} ", i);
        if(i > 0 && (i + 1) % width == 0)
            fmt::print("\n");
    }
    fmt::print("\n");
}

int main()
{
    auto file = std::ifstream{"../day12/input.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    Map map;

    size_t target = 0;
    while(std::getline(file, line))
    {
        width = line.size();
        height += 1;

        for(char c : line)
        {
            if(c == 'S')
            {
                map.push_back(MapNode{.height = 0});
            }
            else if(c == 'E')
            {
                map.push_back(MapNode{.height = 'z' - 'a'});
                target = map.size() -1u;
            }
            else
            {
                map.push_back(MapNode{.height = c - 'a'});
            }
        }
    }

    print(map);

    auto dijkstra = [&map, w = width, h = height](size_t target) {
        fmt::print("BOARD SIZE {} x {}\n", w, h);
        auto findAdjacentNodes = [&](size_t index) {
            std::vector<size_t> ret;
            ret.reserve(4);

            const auto x = index % w;
            const auto y = index / w;
            const auto& node = map.at(index);

            fmt::print("{}-{}:{}\n", index, x, y);

            auto test = [&node](int h)
            {
                return h <= node.height + 1;
            };

            if(x > 0)
            {
                auto idx = index - 1;
                const auto& e = map.at(idx);
                if(test(e.height))
                {
                    ret.emplace_back(idx);
                }
            }
            if(x < w - 1)
            {
                auto idx = index + 1;
                const auto& e = map.at(idx);
                if(test(e.height))
                {
                    ret.emplace_back(idx);
                }
            }
            if(y > 0)
            {
                auto idx = index - w;
                const auto& e = map.at(idx);
                if(test(e.height))
                {
                    ret.emplace_back(idx);
                }
            }
            if(y < h - 1)
            {
                auto idx = index + w;
                const auto& e = map.at(idx);
                if(test(e.height))
                {
                    ret.emplace_back(idx);
                }
            }

            std::sort(std::begin(ret), std::end(ret));
            return ret;
        };

        std::set<std::pair<size_t, int>> set;

        set.emplace(0, 0); // index, distance
        map.at(0).distance = 0;

        int i = 0;
        while(!set.empty())
        {
            i += 1;

            auto [idx, distance] = *set.begin();
            auto node = map.at(idx);
            set.erase(set.begin());
            std::vector<size_t> indices = findAdjacentNodes(idx);
            fmt::print("Round {} - Index {} - Set Size {}\n", i, idx, set.size());
            fmt::print("next indices {}\n", indices);
            for(auto adjacentIdx : indices)
            {
                if(adjacentIdx == target)
                {
                    fmt::print("Min distance is {}\n", node.distance + 1);
                    set.clear();
                    break;
                }

                auto& adj = map.at(adjacentIdx);
                if(adj.distance == -1) // || adj.distance < node.distance)
                {
                    adj.distance = node.distance + 1;
                    adj.prev = idx;

                    std::cout << std::endl;
                    set.emplace(adjacentIdx, adj.distance);
                }
            }
            fmt::print("\n");
        }

        fmt::print("Iterated {} elements\n", i);
    };

    fmt::print("Target Index {}\n", target);
    dijkstra(target);
}
