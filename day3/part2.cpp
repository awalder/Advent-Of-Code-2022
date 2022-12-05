#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <bitset>
#include <array>
#include <assert.h>

int main()
{
    auto file = std::ifstream{"../day3/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    int totalpoints = 0;

    std::string line;
    // std::string groupsack;

    // linenum cycles 0, 1, 2 -> 0, 1, 2 ...
    std::size_t linenum = 0;

    // a,b,c ... x,y,z,A,B,C ... X,Y,Z
    // index + 1 === priority points
    // line 0 sets bit 0001
    // line 1 sets bit 0010
    // line 2 sets bit 0100
    //
    // if all bits are set (0b0111), item is present in all sacks
    std::array<std::bitset<4>, 52> bits;

    auto convert = [](char c) -> std::size_t {
        int res = 0;
        if(std::isupper(c))
            res = static_cast<int>(c) - 38;
        else
            res = static_cast<int>(c) - 96;

        assert(res >= 0 && res <= 52);
        return res;
    };

    while(std::getline(file, line))
    {
        for(char c : line)
        {
            auto p = convert(c);
            bits.at(p - 1).set(linenum);
        }

        linenum += 1;
        if(linenum < 3)
            continue;

        linenum = 0;

        ulong points = 0;
        for(std::size_t i = 0; i < bits.size(); ++i)
        {
            points = bits.at(i).to_ulong();
            if(points == 7)
            {
                totalpoints += i+1;
                break;
            }
        }

        for(auto& b : bits)
        {
            b.reset();
        }
    }

    fmt::print("My score is {}\n", totalpoints);
}
