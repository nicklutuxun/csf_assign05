#include "calc.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

struct Calc {
private:
    // fields
    std::map<std::string, int> var_dict;
public:
    // public member functions
    Calc();
    ~Calc();

    int evalExpr(const std::string &expr, int &result);

    int var_exist(std::string var);
private:
    
};

extern "C" struct Calc *calc_create(void) {
    return new Calc();
}

extern "C" void calc_destroy(struct Calc *calc) {
    delete calc;
}

extern "C" int calc_eval(struct Calc *calc, const char *expr, int *result) {
    return calc->evalExpr(expr, *result);
}

extern "C" int Calc::evalExpr(const std::string &expr, int &result) {
    std::vector<std::string> tokens = tokenize(expr);
    int num_tokens = tokens.size();
    
    switch (num_tokens)
    {
        case 1:
        {
            std::string operand = tokens.at(1);
            if (is_integer(operand) == 1)
            {
                result = std::stoi(operand);
                return 1;       // evaluation succeeds
            }
            else if (is_variable(operand) == 1)
            {
                if (var_exist(operand) == 1)
                {
                    result = this->var_dict.at(operand);
                    return 1;       // evaluation succeeds
                }
            }
            
            break;
        }
        default:
        {
        return 0;
        }
    }
    return 0;
}

extern "C" std::vector<std::string> tokenize(const std::string &expr) {
    std::vector<std::string> vec;
    std::stringstream s(expr);

    std::string tok;
    while (s >> tok) {
        vec.push_back(tok);
    }

    return vec;
}

extern "C" int is_variable(std::string operand) {

    for (std::string::iterator it = operand.begin(); it != operand.end(); it++)
    {
        if (isalpha(*it) == 0)
        {
            return 0;       // operand is not valid variable
        }
        
    }
    
    return 1;       // operand is variable
}

extern "C" int is_integer(std::string operand) {
    for (std::string::iterator it = operand.begin(); it != operand.end(); it++)
    {
        if (isdigit(*it) == 0)
        {
            return 0;       // operand is not valid integer
        }
        
    }

    return 1;       // operand is integer
}

extern "C" int Calc::var_exist(std::string var) {
    if (var_dict.find(var) != var_dict.end())
    {
        return 1;       // variable found
    }
    
    return 0;       // variable not found
}