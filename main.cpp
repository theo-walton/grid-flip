#include "Interpreter.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>

int main(int ac, char** av)
{
    if (ac != 2)
    {
        std::cout << "program needs input file" << std::endl;
        return 0;
    }
    std::ifstream f;
    std::stringstream ss;
    f.open(av[1]);
    if (f.fail() || !f.good())
	{
		std::cerr << av[0] << ": " << av[1] << ": " << strerror(errno) << std::endl;
		return 1;
	}
    ss << f.rdbuf();
    Interpreter interpret(ss.str());


    interpret.Run(10000000);
    interpret.Print();
}
