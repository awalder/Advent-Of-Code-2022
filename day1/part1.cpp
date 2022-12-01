#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>

int main()
{
    auto file = std::ifstream{"../day1/example1.txt", std::ios::in};

    int topcalories = 0;
    int cals = 0;

    if(file)
    {
        std::string line;
        while(std::getline(file, line))
        {
            if(line.empty())
            {
                cals = 0;
            }
            std::stringstream ss(line);

            int val = 0;
            ss >> val;

            cals += val;
            if(cals > topcalories)
            {
                topcalories = cals;
            }
        }
    }

    fmt::print("Top cals: {}\n", topcalories);
}
