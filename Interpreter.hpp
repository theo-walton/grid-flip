#pragma once

#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

class Interpreter
{
    static constexpr int _size = 50;
    bool _grid[_size][_size] = {{false}};
    int _x;
    int _y;
    std::vector<std::function<void()>> _commands;
    int _index;

    std::function<void()> gen_setx(const std::string& command);
    std::function<void()> gen_sety(const std::string& command);

    // checks the _grid[_x % _size][_y % _size] bit, and if true, skips
    // specified number of commands
    std::function<void()> gen_if(const std::string& spaces);

    // flips the _grid[_x % _size][_y % _size] bit
    std::function<void()> gen_flip();


public:
    Interpreter(const std::string& code);

    void Run(int iterations);
    void Print();
};
