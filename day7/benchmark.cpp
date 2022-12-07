#include <fstream>
#include <numeric>
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
#include <charconv>
#include <functional>
#include <benchmark/benchmark.h>
#include <assert.h>

using DirList = std::vector<std::pair<std::string, size_t>>;

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

auto sum(Dir* node, DirList& list) -> void
{
    // Bottom up
    for(const auto& d : node->dirs)
    {
        sum(d.get(), list);
    }

    if(node->parent)
    {
        node->parent->size += node->size;
    }

    list.emplace_back(node->name, node->size);
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

static void BM_Antti_DAY7(benchmark::State& state)
{
    struct Pokale
    {
        Pokale(std::string nem,
               std::function<int(std::vector<std::string>&)>&& getDirSize) :
            name(nem), sizeOp(std::move(getDirSize))
        {
        }
        int getSize()
        {
            int sizeOpSize = 0;
            if(size == 0)
            {
                for(const auto& [path, fileSize] : files)
                {
                    size += fileSize;
                }
                sizeOpSize = sizeOp(directories);
                size += sizeOpSize;
            }
            return size;
        }
        std::string name;
        std::map<std::string, int> files{};
        std::vector<std::string> directories;
        std::function<int(std::vector<std::string>&)> sizeOp;
        int size = 0;
    };
    auto makeKey = [](auto& path) -> std::string {
        std::string key;
        for(auto a : path)
        {
            key += std::string("/") + a;
        }
        if(key.empty())
            return "/";
        return key;
    };
    std::stringstream ss;
    auto file = std::ifstream{"../day7/input1.txt", std::ios::in};
    ss << file.rdbuf();
    std::string line;
    std::vector<std::string> path;
    std::map<std::string, std::shared_ptr<Pokale>> filesystem;
    auto current = filesystem.end();
    int result = 0;

    for(auto _ : state)
    {
        ss.seekg(0);
        while(std::getline(ss, line))
        {
            if(line.front() == '$')
            {
                auto pos = line.find(" ");
                auto cmdStr = line.substr(pos + 1);
                if(cmdStr == "ls")
                {
                    bool succ = false;
                    std::tie(current, succ) = filesystem.insert(
                            {makeKey(path),
                             std::make_shared<Pokale>(
                                     makeKey(path),
                                     [&](std::vector<std::string>& dirs)
                                             -> int {
                                         int total = 0;
                                         for(auto& path : dirs)
                                         {
                                             auto it = filesystem.find(path);
                                             if(it != filesystem.end())
                                                 total += it->second->getSize();
                                         }
                                         return total;
                                     })});
                }
                else
                {
                    pos = cmdStr.rfind(" ");
                    auto target = cmdStr.substr(pos + 1);
                    if(target == "/")
                    {
                        path.clear();
                    }
                    else if(target == "..")
                    {
                        path.pop_back();
                    }
                    else
                    {
                        path.push_back(target);
                    }
                }
            }
            else
            {
                auto pos = line.find(" ");
                auto first = line.substr(0, pos);
                auto filename = line.substr(pos + 1);
                if(first == "dir")
                {
                    if(path.empty())
                    {
                        current->second->directories.push_back("/" + filename);
                    }
                    else
                    {
                        current->second->directories.push_back(
                                makeKey(path) + "/" + filename);
                    }
                }
                else
                {
                    if(path.empty())
                    {
                        current->second->files["/" + filename] =
                                std::stoi(first);
                    }
                    else
                    {
                        current->second->files[makeKey(path) + "/" + filename] =
                                std::stoi(first);
                    }
                }
            }
        }
        auto full = filesystem["/"]->getSize();
        std::set<int> sizes;
        for(auto& [path, file] : filesystem)
        {
            int total = file->getSize();
            sizes.insert(total);
        }
        auto needed = 30000000 - (70000000 - full);
        for(auto it = sizes.begin(); it != sizes.end(); ++it)
        {
            if(*it > needed)
            {
                result = *it;
                assert(result == 6183184);
                benchmark::DoNotOptimize(result);
                break;
            }
        }
    }
}

static void BM_Anssi_DAY7(benchmark::State& state)
{
    auto file = std::ifstream{"../day7/input1.txt", std::ios::in};

    std::unique_ptr<Dir> root = std::make_unique<Dir>("root", nullptr);
    Dir* current = root.get();

    std::string line;
    while(std::getline(file, line))
    {
        if(line.empty())
            continue;
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
            }
        }
        else if(tok.at(0) == "dir")
        { // Enter command
        }
        else
        {
            size_t size = std::stoi(std::string(tok.at(0)));
            current->size += size;
        }
    }

    DirList list;
    sum(root.get(), list);

    // std::sort(list.begin(), list.end(), [](const auto& lhs, const auto& rhs)
    // {
    //     const auto& [lname, lsize] = lhs;
    //     const auto& [rname, rsize] = rhs;
    //     return lsize < rsize;
    // });

    const size_t freespace = 70'000'000 - root->size;
    const size_t required = 30'000'000;

    fmt::print("Used memory {}\n", root->size);
    fmt::print("Free memory {}\n", freespace);
    fmt::print("Required memory {}\n", required);
    fmt::print("Need to free at least {}\n", required - freespace);

    size_t result = std::numeric_limits<size_t>::max();
    for(auto& [name, size] : list)
    {
        if(size >= required - freespace && size < result)
        {
            result = size;
        }
    }

    fmt::print("Result {}\n", result);

    for(auto _ : state)
    {
    }

    assert(result == 6183184);
}

BENCHMARK(BM_Antti_DAY7);
BENCHMARK(BM_Anssi_DAY7);
BENCHMARK_MAIN();
