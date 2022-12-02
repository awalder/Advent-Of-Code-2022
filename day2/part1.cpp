#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>

// A Rock
// B Paper
// C Scissors
//
// Scores:
// 1 for Rock
// 2 for Paper
// 3 for Scissors
//
// 0 for lost round
// 3 for draw
// 6 for Win

enum struct Weapon
{
    ROCK,
    PAPER,
    SCISSORS,
};

auto to_string(Weapon w)
{
    switch(w)
    {
    case Weapon::ROCK: return "ROCK";
    case Weapon::PAPER: return "PAPER";
    case Weapon::SCISSORS: return "SCISSORS";
    default: return "";
    }
}

auto playRPS(Weapon other, Weapon me) -> int
{

    if(other == Weapon::ROCK && me == Weapon::ROCK)
    { // draw
        return 3 + 1;
    }
    else if(other == Weapon::ROCK && me == Weapon::PAPER)
    {
        return 6 + 2;
    }
    else if(other == Weapon::ROCK && me == Weapon::SCISSORS)
    {
        return 0 + 3;
    }
    else if(other == Weapon::PAPER && me == Weapon::ROCK)
    {
        return 0 + 1;
    }
    else if(other == Weapon::PAPER && me == Weapon::PAPER)
    {
        return 3 + 2;
    }
    else if(other == Weapon::PAPER && me == Weapon::SCISSORS)
    {
        return 6 + 3;
    }
    else if(other == Weapon::SCISSORS && me == Weapon::ROCK)
    {
        return 6 + 1;
    }
    else if(other == Weapon::SCISSORS && me == Weapon::PAPER)
    {
        return 0 + 2;
    }
    else if(other == Weapon::SCISSORS && me == Weapon::SCISSORS)
    {
        return 3 + 3;
    }

    return 0;
}

int main()
{
    auto file = std::ifstream{"../day2/input1.txt", std::ios::in};

    if(!file)
    {
        fmt::print("No file\n");
        return 0;
    }

    auto getWeapon = [](char c) {
        if(c == 'A' || c == 'X')
            return Weapon::ROCK;
        else if(c == 'B' || c == 'Y')
            return Weapon::PAPER;
        else
            return Weapon::SCISSORS;
    };

    std::string line;
    int round = 1;
    int totalscore = 0;
    while(std::getline(file, line))
    {
        std::stringstream ss(line);

        char a, b;
        ss >> a >> b;
        auto other = getWeapon(a);
        auto me = getWeapon(b);

        auto score = playRPS(other, me);

        fmt::print(
                "Round [{}], {} : {} => {}\n",
                round,
                to_string(other),
                to_string(me),
                score);
        round += 1;
        totalscore += score;
    }

    fmt::print("My score is {}\n", totalscore);
}
