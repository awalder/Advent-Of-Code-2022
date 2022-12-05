#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>

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
    while(std::getline(file, line))
    {
        // std::stringstream ss(line);

        auto n = line.size() / 2;
        std::string a, b;
        a.resize(n);
        b.resize(n);
        std::copy_n(line.begin(), n, a.begin());
        std::copy_n(line.begin() + n, n, b.begin());

        char common = 0;
        for(char ca : a)
        {
            for(char cb : b)
            {
                if (ca == cb)
                {
                    common = ca;
                    goto found;
                }
            }
        }

found:
        int points = 0;
        if(std::isupper(common))
        {
            points = static_cast<int>(common) - 64 + 26;
        }
        else {
            points = static_cast<int>(common) - 96;
        }
        totalpoints += points;
        fmt::print("common({}): points({})\n", common, points);
    }

    fmt::print("My score is {}\n", totalpoints);
}
