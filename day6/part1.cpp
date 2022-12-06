#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <algorithm>
#include <array>
#include <string_view>

int main()
{
    auto file = std::ifstream{"../day6/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    std::array<char, 4> buffer;
    size_t index = 0;

    while(std::getline(file, line))
    {
        for(char c : line)
        {
            buffer.at(index++ % 4) = c;

            // still reading characters
            if(index < 4)
                continue;

            auto tmp = buffer;
            std::sort(tmp.begin(), tmp.end());
            auto res = std::adjacent_find(
                    tmp.begin(), tmp.end(), std::equal_to<char>{});

            if(res == tmp.end())
            {
                fmt::print("Marker found at {}\n", index);
                index = 0;
                break;
            }
        }
    }
    fmt::print("\n");
}
