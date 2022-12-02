#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <fmt/printf.h>

// A Rock
// B Paper
// C Scissors
//
// X Lose
// Y Draw
// Z Win
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

enum struct Action
{
    LOSE,
    DRAW,
    WIN,
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

auto to_string(Action w)
{
    switch(w)
    {
    case Action::LOSE: return "LOSE";
    case Action::DRAW: return "DRAW";
    case Action::WIN: return "WIN";
    default: return "";
    }
}

auto playRPS(Weapon other, Action me) -> int
{
    if(other == Weapon::ROCK && me == Action::WIN) // paper
        return 6 + 2;
    else if(other == Weapon::ROCK && me == Action::DRAW) // rock
        return 3 + 1;
    else if(other == Weapon::ROCK && me == Action::LOSE) // scissors
        return 0 + 3;

    else if(other == Weapon::PAPER && me == Action::WIN) // scissors
        return 6 + 3;
    else if(other == Weapon::PAPER && me == Action::DRAW) // paper
        return 3 + 2;
    else if(other == Weapon::PAPER && me == Action::LOSE) // rock
        return 0 + 1;

    else if(other == Weapon::SCISSORS && me == Action::WIN) // rock
        return 6 + 1;
    else if(other == Weapon::SCISSORS && me == Action::DRAW) // scissors
        return 3 + 3;
    else if(other == Weapon::SCISSORS && me == Action::LOSE) // paper
        return 0 + 2;

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

    auto getAction = [](char c) {
        if(c == 'A' || c == 'X')
            return Action::LOSE;
        else if(c == 'B' || c == 'Y')
            return Action::DRAW;
        else
            return Action::WIN;
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
        auto me = getAction(b);

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
