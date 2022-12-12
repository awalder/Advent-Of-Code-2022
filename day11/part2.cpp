#include <charconv>
#include <cmath>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <range/v3/all.hpp>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Monkey
{
    std::vector<std::int64_t> items; // value == worrylevel
    std::function<int(int)> operation;
    std::function<int(int)> test;
    size_t monkeyTrue = 0;
    size_t monkeyFalse = 0;
    int inspectCount = 0;
};

using Monkeys = std::vector<Monkey>;

auto print(const Monkeys& monkeys)
{
    size_t i = 0;
    for(const auto& monkey : monkeys)
    {
        fmt::print(
                "Monkey {}: {} -- {}\n",
                i++,
                monkey.items.size(),
                monkey.items);
    }
}

int main()
{
    auto file = std::ifstream{"../day11/example.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    namespace rv = ranges::views;
    std::string line;
    Monkeys monkeys;

    auto to_int = [](const std::string& str) {
        auto begin = str.data();
        auto end = str.data() + str.size();
        std::int64_t value = 0;
        std::from_chars(begin, end, value);
        return value;
    };

    static const std::regex r{"[\\w+*]+"};

    while(std::getline(file, line))
    {
        if(line.starts_with("Monkey"))
        {
            // fmt::print("Monkey {}\n", monkeys.size());
            monkeys.emplace_back();
        }

        assert(monkeys.size() > 0);
        auto& monkey = monkeys.back();

        if(line.starts_with("  Starting items:"))
        {
            auto pos = line.find_first_of(':');
            auto str = line.substr(pos + 1);
            monkey.items = str | rv::tokenize(r) | rv::transform(to_int)
                           | ranges::to_vector;
            // fmt::print("Items: {}\n", monkey.items);
        }
        else if(line.starts_with("  Operation:"))
        {
            auto rng = line | rv::tokenize(r) | rv::drop(3) | ranges::to_vector;
            monkey.operation =
                    [op = std::string(rng.at(0)),
                     rhs = std::string(rng.at(1))](int old) -> std::int64_t {
                std::int64_t b = rhs == "old" ? old : std::stoi(rhs);
                std::int64_t res = op == "*" ? old * b : old + b;
                return res;
            };
        }
        else if(line.starts_with("  Test"))
        {
            auto rng =
                    line | rv::tokenize(r) | ranges::copy | ranges::to_vector;
            monkey.test = [divisor = std::stoi(rng.at(3))](std::int64_t value) -> bool {
                fmt::print(
                        "TEST: divide {} % {} = {}\n",
                        value,
                        divisor,
                        value % divisor == 0);
                return value % divisor == 0;
            };
        }
        else if(line.starts_with("    If true"))
        {
            auto rng = line | rv::tokenize(r) | ranges::to_vector;
            monkey.monkeyTrue = std::stoi(rng.at(5));
        }
        else if(line.starts_with("    If false"))
        {
            auto rng = line | rv::tokenize(r) | ranges::to_vector;
            monkey.monkeyFalse = std::stoi(rng.at(5));
        }
    }

    print(monkeys);

    // 10000 rounds
    for(int i = 0; i < 10'000; ++i)
    {
        fmt::print("****** BEGIN ******\n");
        fmt::print("***** ROUND {} ******\n", i);
        for(auto& monkey : monkeys)
        {
            auto rng = monkey.items | rv::transform(monkey.operation);
            std::cout << rng << std::endl;
            monkey.inspectCount += rng.size();

            ranges::for_each(rng, [&monkeys, &monkey](std::int64_t item) {
                auto receiver = monkey.test(item) ? monkey.monkeyTrue
                                                  : monkey.monkeyFalse;
                fmt::print("give item {} to monkey {}\n", item, receiver);
                monkeys.at(receiver).items.emplace_back(item);
            });

            monkey.items.clear();
        }

        print(monkeys);
    }

    int i = 0;
    std::vector<std::int64_t> counts;
    for(auto& monkey : monkeys)
    {
        counts.emplace_back(monkey.inspectCount);
        fmt::print(
                "Monkey {} inspected items {} times.\n",
                i++,
                monkey.inspectCount);
    }

    ranges::sort(counts);
    auto res = ranges::accumulate(
            counts | rv::reverse | rv::take(2),
            1u,
            std::multiplies<std::int64_t>{});

    fmt::print("Result of {} -> {}\n", counts, res);
}
