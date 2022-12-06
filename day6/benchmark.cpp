#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>
#include <cctype>
#include <algorithm>
#include <array>
#include <string_view>
#include <deque>
#include <benchmark/benchmark.h>

#include "tracy/Tracy.hpp"

void swapping(char &a, char &b) {      //swap the content of a and b
   int temp;
   temp = a;
   a = b;
   b = temp;
}

void bubbleSort(char *array, int size) {
   for(int i = 0; i<size; i++) {
      int swaps = 0;         //flag to detect any swap is there or not
      for(int j = 0; j<size-i-1; j++) {
         if(array[j] > array[j+1]) {       //when the current item is bigger than next
            swapping(array[j], array[j+1]);
            swaps = 1;    //set swap flag
         }
      }
      if(!swaps)
         break;       // No swap in this pass, so array is sorted
   }
}

inline auto doThings(uint8_t chr) -> uint32_t
{
    return 1 << ((chr & 0x1F) - 1);
}

struct Part2
{
    static const int BUFFER_SIZE = 1;
    std::string result;
    std::fstream stream;
    std::stringstream input;
    size_t markerWidth = 0;

    Part2(size_t width) :
        stream("../day6/input1.txt", stream.binary | stream.in),
        markerWidth(width)
    {
        input << stream.rdbuf();
    }

    inline void moveToBegin() { stream.seekg(0); }

    explicit operator bool() const { return stream.is_open(); }
    std::string toString() const
    {
        return "day6/part2 width=" + std::to_string(markerWidth)
               + " result=" + result;
    }

    std::array<uint32_t, 14> values;
    size_t index = 0;
    bool checkOp(uint8_t chr)
    {
        values.at(index++ % markerWidth) = doThings(chr);

        auto result =
                !((values[0] & values[1]) || (values[0] & values[2])
                  || (values[0] & values[3]) || (values[0] & values[4])
                  || (values[0] & values[5]) || (values[0] & values[6])
                  || (values[0] & values[7]) || (values[0] & values[8])
                  || (values[0] & values[9]) || (values[0] & values[10])
                  || (values[0] & values[11]) || (values[0] & values[12])
                  || (values[0] & values[13]) ||

                  (values[1] & values[2]) || (values[1] & values[3])
                  || (values[1] & values[4]) || (values[1] & values[5])
                  || (values[1] & values[6]) || (values[1] & values[7])
                  || (values[1] & values[8]) || (values[1] & values[9])
                  || (values[1] & values[10]) || (values[1] & values[11])
                  || (values[1] & values[12]) || (values[1] & values[13]) ||

                  (values[2] & values[3]) || (values[2] & values[4])
                  || (values[2] & values[5]) || (values[2] & values[6])
                  || (values[2] & values[7]) || (values[2] & values[8])
                  || (values[2] & values[9]) || (values[2] & values[10])
                  || (values[2] & values[11]) || (values[2] & values[12])
                  || (values[2] & values[13]) ||

                  (values[3] & values[4]) || (values[3] & values[5])
                  || (values[3] & values[6]) || (values[3] & values[7])
                  || (values[3] & values[8]) || (values[3] & values[9])
                  || (values[3] & values[10]) || (values[3] & values[11])
                  || (values[3] & values[12]) || (values[3] & values[13]) ||

                  (values[4] & values[5]) || (values[4] & values[6])
                  || (values[4] & values[7]) || (values[4] & values[8])
                  || (values[4] & values[9]) || (values[4] & values[10])
                  || (values[4] & values[11]) || (values[4] & values[12])
                  || (values[4] & values[13]) ||

                  (values[5] & values[6]) || (values[5] & values[7])
                  || (values[5] & values[8]) || (values[5] & values[9])
                  || (values[5] & values[10]) || (values[5] & values[11])
                  || (values[5] & values[12]) || (values[5] & values[13])
                  || (values[6] & values[7]) || (values[6] & values[8])
                  || (values[6] & values[9]) || (values[6] & values[10])
                  || (values[6] & values[11]) || (values[6] & values[12])
                  || (values[6] & values[13]) ||

                  (values[7] & values[8]) || (values[7] & values[9])
                  || (values[7] & values[10]) || (values[7] & values[11])
                  || (values[7] & values[12]) || (values[7] & values[13]) ||

                  (values[8] & values[9]) || (values[8] & values[10])
                  || (values[8] & values[11]) || (values[8] & values[12])
                  || (values[8] & values[13]) ||

                  (values[9] & values[10]) || (values[9] & values[11])
                  || (values[9] & values[12]) || (values[9] & values[13]) ||

                  (values[10] & values[11]) || (values[10] & values[12])
                  || (values[10] & values[13]) ||

                  (values[11] & values[12]) || (values[11] & values[13]) ||

                  (values[12] & values[13]));
        return result;
    }

    void run()
    {
        std::array<uint8_t, BUFFER_SIZE> buffer{0};
        int count = 0;
        for(auto size = input.readsome((char*)buffer.data(), BUFFER_SIZE);
            size > 0;)
        {
            ++count;
            if(checkOp(buffer.front()))
            {
                // result += std::to_string(count) + " ";
                break;
            }
            size = input.readsome((char*)buffer.data(), BUFFER_SIZE);
        }
    }
};

// static void BM_dirty(benchmark::State& state)
// {
//     ZoneScopedN("Antti");
//     Part2 part2(14);
//     for(auto _ : state)
//     {
//         FrameMarkNamed("Dirty");
//         part2.moveToBegin();
//         part2.run();
//     }
// }

static void BM_Anssi_DAY6(benchmark::State& state)
{
    ZoneScopedN("Anssi");
    auto file = std::ifstream{"../day6/input1.txt", std::ios::in};

    std::stringstream input;
    input << file.rdbuf();
    std::string line;
    std::array<char, 14> buffer;
    std::array<char, 14> tmp;
    size_t index = 0;

    for(auto _ : state)
    {
        FrameMarkNamed("Pure");
        input.seekg(0);
        while(std::getline(input, line))
        {
            for(char c : line)
            {
                buffer.at(index++ % 14) = c;

                // still reading characters
                if(index < 14)
                    continue;

                tmp = buffer;
                std::sort(tmp.begin(), tmp.end());
                auto res = std::adjacent_find(
                        tmp.begin(), tmp.end(), std::equal_to<char>{});

                if(res == tmp.end())
                {
                    // fmt::print("Marker found at {}\n", index);
                    index = 0;
                    break;
                }
            }
        }
    }
}

// BENCHMARK(BM_simple);
BENCHMARK(BM_Anssi_DAY6);
BENCHMARK_MAIN();
