#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <algorithm>

int main()
{
    auto file = std::ifstream{"../day4/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    std::string line;
    int count = 0;
    while(std::getline(file, line))
    {
        std::stringstream ss(line);

        int a1, a2, b1, b2;
        char dummy;

        ss >> a1 >> dummy >> a2 >> dummy >> b1 >> dummy >> b2;
        fmt::print("{}-{},{}-{} ", a1, a2, b1, b2);

        if(!(a2 < b1 || b2 < a1))
        {
            fmt::print(" <-");
            count += 1;
        }

        fmt::print("\n");
    }

    fmt::print("{} intersections\n", count);
}
