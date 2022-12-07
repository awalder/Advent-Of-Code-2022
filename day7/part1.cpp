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
#include <fmt/ostream.h>
#include <tuple>

struct Dir
{
    Dir(std::string_view name, Dir* parent) : name(name), parent(parent) {}
    // size of this directory and everything under it
    std::string name;
    size_t size;
    Dir* parent = nullptr;
    std::vector<std::unique_ptr<Dir>> dirs;

    auto has(std::string_view name) -> Dir*
    {
        for(const auto& d : dirs)
        {
            if(d->name == name)
                return d.get();
        }
        return nullptr;
    }
};

auto print(Dir* node) -> void
{
    fmt::print("Directory {} : size {}\n", node->name, node->size);
    for(const auto& d : node->dirs)
    {
        print(d.get());
    }
}

auto sum(Dir* node, int& res) -> void
{
    // Bottom up
    for(const auto& d : node->dirs)
    {
        sum(d.get(), res);
    }

    if(node->parent)
    {
        node->parent->size += node->size;
    }

    if(node->size <= 100'000)
    {
        res += node->size;
    }
}

auto tokenize(const std::string& input, const char* delim = " \n")
        -> std::vector<std::string_view>
{
    std::vector<std::string_view> res;
    size_t pos = 0;
    auto sw = std::string_view{input};
    while(true)
    {
        if(auto size = sw.find_first_of(delim); size != std::string_view::npos)
        {
            res.emplace_back(sw.data(), size);

            if(pos + size + 1 >= input.size())
                break;

            pos += size + 1;
            sw = std::string_view{&input.at(pos), input.size() - pos};
        }
        else if(pos < input.size())
        {
            res.emplace_back(&input.at(pos), input.size() - pos);
            break;
        }
        else
        {
            break;
        }
    }

    return res;
}

int main()
{
    auto file = std::ifstream{"../day7/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::unique_ptr<Dir> root = std::make_unique<Dir>("root", nullptr);
    Dir* current = root.get();

    std::string line;
    while(std::getline(file, line))
    {
        auto tok = tokenize(line);

        if(tok.at(0) == "$")
        { // Enter command
            if(tok.at(1) == "cd")
            { // Change directory
                if(tok.at(2) == "..")
                { // Move up
                    current = current->parent;
                }
                else
                { // Move into
                    auto name = tok.at(2);
                    auto next = current->has(name);
                    if(!next)
                    {
                        current->dirs.emplace_back(
                                std::make_unique<Dir>(name, current));
                        current = current->dirs.back().get();
                    }
                    else
                    {
                        current = next;
                    }
                }
            }
            else if(tok.at(1) == "ls")
            {
                fmt::print("List files of {}\n", current->name);
            }
        }
        else if(tok.at(0) == "dir")
        { // Enter command
          // fmt::print("List files of directory {}\n", tok.at(1));
        }
        else
        {
            size_t size = std::stoi(std::string(tok.at(0)));
            current->size += size;
            fmt::print("File {}:{}\n", tok.at(1), size);
        }
    }

    int result = 0;

    fmt::print("#########################\n");
    print(root.get());
    sum(root.get(), result);
    fmt::print("#########################\n");
    print(root.get());

    fmt::print("Sum of sizes of directories under 100'000B :: {}\n", result);

}
