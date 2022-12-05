#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <algorithm>

using Stack = std::vector<std::vector<char>>;

auto move(Stack& stack, size_t from, size_t to, size_t n)
{
    auto& f = stack.at(from);
    auto& t = stack.at(to);
    auto begin = f.end() - n;
    auto end = f.end();
    std::copy(begin, end, std::back_inserter(t));
    f.erase(begin, end);
}

auto print(const Stack& stack)
{
    for(const auto& s : stack)
    {
        for(const auto& v : s)
        {
            fmt::print("[{}] ", v);
        }
        fmt::print("\n");
    }
}

struct Command
{
    size_t n = 0;
    size_t from = 0;
    size_t to = 0;
};

using Commands = std::vector<Command>;

int main()
{
    auto file = std::ifstream{"../day5/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    Stack stack(10);
    Commands commands;

    while(std::getline(file, line))
    {
        std::stringstream ss(line);
        // fmt::print("{}\n", line);

        if(!line.starts_with("move"))
        {
            for(size_t i = 0; i < line.size(); ++i)
            {
                char c = line.at(i);
                size_t charIndex = (i - 1) / 4;
                if(std::isalpha(c) && std::isupper(c))
                {
                    stack.at(charIndex).emplace_back(c);
                }
            }
            continue;
        }

        size_t n, from, to;
        char c;

        ss >> c >> c >> c >> c; // move
        ss >> n;
        ss >> c >> c >> c >> c; // from
        ss >> from;
        ss >> c >> c; // to
        ss >> to;

        // fmt::print("move {} from {} to {} <-\n", n, from, to);

        commands.push_back({n, from - 1u, to - 1u});
    }

    // reverse, as items were read from top to bottom
    for(auto& s : stack)
    {
        std::reverse(s.begin(), s.end());
    }

    // print(stack);

    for(auto& c : commands)
    {
        move(stack, c.from, c.to, c.n);
        // print(stack);
    }

    // print(stack);

    for(const auto& s : stack)
    {
        if(s.empty())
            continue;
        fmt::print("{}", s.back());
    }
    fmt::print("\n");
}
