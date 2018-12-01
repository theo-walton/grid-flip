#include "Interpreter.hpp"

#define TM(X) true_mod(X, _size)

#define LAMBDA_RETURN(_NUM1_, _NUM2_, _OP_, _VAR_) \
{\
    if (x1 && x2)\
        return [this](){_VAR_ = TM(_x) _OP_ TM(_x);};\
    if (x1 && y2)\
        return [this](){_VAR_ = TM(_x) _OP_ TM(_y);};\
    if (x1)\
        return [this, _NUM2_](){_VAR_ = TM(_x) _OP_ TM(_NUM2_);};\
    if (y1 && x2)\
        return [this](){_VAR_ = TM(_y) _OP_ TM(_x);};\
    if (y1 && y2)\
        return [this](){_VAR_ = TM(_y) _OP_ TM(_y);};\
    if (y1)\
        return [this, _NUM2_](){_VAR_ = TM(_y) _OP_ TM(_NUM2_);};\
    if (x2)\
        return [this, _NUM1_](){_VAR_ = TM(_NUM1_) _OP_ TM(_x);};\
    if (y2)\
        return [this, _NUM1_](){_VAR_ = TM(_NUM1_) _OP_ TM(_y);};\
    return [this, _NUM1_, _NUM2_](){_VAR_ = TM(_NUM1_) _OP_ TM(_NUM2_);};\
}

static int true_mod(int n, int a)
{
    int out = n % a;
    if (out < 0)
        out += a;
    return out;
}

std::function<void()> Interpreter::gen_setx(const std::string& command)
{
    if (command.length() == 0)
        throw std::runtime_error("empty command");
    size_t index = 0;
    bool x1 = false;
    bool y1 = false;
    int num1;
    char op = '\0';
    bool x2 = false;
    bool y2 = false;
    int num2;

    if (command[index] == 'x')
    {
        index++;
        x1 = true;
    }
    else if (command[index] == 'y')
    {
        index++;
        y1 = true;
    }
    else
    {
        size_t pos;
        num1 = std::stoi(command.substr(index), &pos);
        index += pos;
    }

    if (index == command.length())
        ;
    else
    {
        op = command[index];
        index++;
        if (command.length() == 0)
            throw std::runtime_error("variable required after operator");
        if (command[index] == 'x')
        {
            index++;
            x2 = true;
        }
        else if (command[index] == 'y')
        {
            index++;
            y2 = true;
        }
        else
        {
            size_t pos;
            num2 = std::stoi(command.substr(index), &pos);
            index += pos;
        }
        if (index != command.length())
            throw std::runtime_error("extra characters at end of command");
    }

    if (op == '\0')
    {
        if (x1)
            return [](){};
        if (y1)
            return [this](){_x = _y;};
        return [this, num1](){_x = num1;};
    }
    if (op == '*')
        LAMBDA_RETURN(num1, num2, *, _x)
    if (op == '+')
        LAMBDA_RETURN(num1, num2, +, _x)
    if (op == '-')
        LAMBDA_RETURN(num1, num2, -, _x)
    return [](){};
}

std::function<void()> Interpreter::gen_sety(const std::string& command)
{
    if (command.length() == 0)
        throw std::runtime_error("empty command");
    size_t index = 0;
    bool x1 = false;
    bool y1 = false;
    int num1;
    char op = '\0';
    bool x2 = false;
    bool y2 = false;
    int num2;

    if (command[index] == 'x')
    {
        index++;
        x1 = true;
    }
    else if (command[index] == 'y')
    {
        index++;
        y1 = true;
    }
    else
    {
        size_t pos;
        num1 = std::stoi(command.substr(index), &pos);
        index += pos;
    }

    if (index == command.length())
        ;
    else
    {
        op = command[index];
        index++;
        if (command.length() == 0)
            throw std::runtime_error("variable required after operator");
        if (command[index] == 'x')
        {
            index++;
            x2 = true;
        }
        else if (command[index] == 'y')
        {
            index++;
            y2 = true;
        }
        else
        {
            size_t pos;
            num2 = std::stoi(command.substr(index), &pos);
            index += pos;
        }
        if (index != command.length())
            throw std::runtime_error("extra characters at end of command");
    }

    if (op == '\0')
    {
        if (x1)
            return [](){};
        if (y1)
            return [this](){_x = _y;};
        return [this, num1](){_x = num1;};
    }
    if (op == '*')
        LAMBDA_RETURN(num1, num2, *, _y)
    if (op == '+')
        LAMBDA_RETURN(num1, num2, +, _y)
    if (op == '-')
        LAMBDA_RETURN(num1, num2, -, _y)
    return [](){};
}

std::function<void()> Interpreter::gen_if(const std::string& spaces)
{
    int space_num = std::stoi(spaces);
    return [this, space_num]()
    {
        if(_grid[true_mod(_x, _size)][true_mod(_y, _size)])
            _index += space_num;
    };
}

std::function<void()> Interpreter::gen_flip()
{
    return [this]()
    {
        _grid[true_mod(_x, _size)][true_mod(_y, _size)] ^= true;
    };
}

static std::vector<std::string> split_semicolon(std::string s)
{
    size_t pos = 0;
    std::string token;
    std::vector<std::string> out;
    while ((pos = s.find(";")) != std::string::npos)
    {
        token = s.substr(0, pos);
        out.push_back(token);
        s.erase(0, pos + 1);
    }
    return out;
}

static std::vector<std::string> split_whitespace(std::string s)
{
    std::vector<std::string> out;
    std::istringstream iss(s);
    for (std::string s; iss >> s;)
        out.push_back(s);
        return out;
}

static std::vector<std::vector<std::string>> get_tokens(std::string str)
{
    std::vector<std::vector<std::string>> out;
    auto lines = split_semicolon(str);
    for (std::string line : lines)
        out.push_back(split_whitespace(line));
    return out;
}

Interpreter::Interpreter(const std::string& code)
{
    _x = 0;
    _y = 0;
    _index = 0;

    std::vector<std::vector<std::string>> tokens = get_tokens(code);
    for (auto& token : tokens)
    {
        std::cout << "token: " << token[0] << std::endl;
        if (token[0] == "setx")
        {
            if (token.size() != 2)
                throw std::runtime_error("setx must be followed by single token");
            _commands.push_back(gen_setx(token[1]));
        }
        else if (token[0] == "sety")
        {
            if (token.size() != 2)
                throw std::runtime_error("sety must be followed by single token");
            _commands.push_back(gen_sety(token[1]));
        }
        else if (token[0] == "if")
        {
            if (token.size() != 2)
                throw std::runtime_error("if must be followed by single token");
            _commands.push_back(gen_if(token[1]));
        }
        else if (token[0] == "flip")
        {
            if (token.size() != 1)
                throw std::runtime_error("flip cannot be followed by a token");
            _commands.push_back(gen_flip());
        }
        _commands.back()();
        std::cout << "success" << std::endl;
    }
}

void Interpreter::Run(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        _commands[true_mod(_index, _commands.size())]();
        _index++;
    }
}

void Interpreter::Print()
{
    for (int x = 0; x < _size; x++)
    {
        for (int y = 0; y < _size; y++)
        {
            std::cout << _grid[x][y] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "x: " << _x << ",y: " << _y << std::endl;
}
