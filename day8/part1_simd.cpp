#include <fstream>
#include <sstream>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <string_view>
#include <numeric>
#include <cmath>
#include <immintrin.h>
// #include <intrin.h>
#include <vector>

using Grid = std::vector<__m256i>;

auto print(const Grid& grid)
{
    for(const auto& row : grid)
    {
        for(size_t i = 0; i < 8; ++i)
        {
            fmt::print("{} ", static_cast<int>(row[i]));
        }
        fmt::print("\n");
    }
}

size_t width = 0;
size_t height = 0;

int main()
{
    auto file = std::ifstream{"../day8/example.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    // TreeGrid grid;
    std::string line;

    std::vector<__m256i> data;

    while(std::getline(file, line))
    {
        data.emplace_back(_mm256_setzero_si256());
        auto& back = data.back();
        back = _mm256_set_epi32(1, 2, 3, 4, 5, 6, 7, 8);
        width = line.length();
        height += 1;

        // size_t i = 0;
        // for(char c : line)
        // {
        //     back[i++] = static_cast<int>(c - '0');
        // }
    }

    print(data);

    // fmt::print("{} ", data[0]);
}
