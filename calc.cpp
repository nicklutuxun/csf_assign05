#include "calc.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

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
    std::vector<std::string> tokenize(const std::string &expr);

    int is_variable(std::string operand);

    int is_integer(std::string operand);

    int is_operand(std::string operand);

    int is_operator(std::string op);
};

Calc::Calc() {}

Calc::~Calc() {}

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
            std::string operand = tokens.at(0);
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

        case 3:
        {
            std::string operand1 = tokens.at(0);
            std::string op = tokens.at(1);
            std::string operand2 = tokens.at(2);
            
            if (is_integer(operand1) == 1 && is_integer(operand2) == 1 && is_operator(op) == 1)
            {
                switch (op[0])
                {
                case '+':
                    result = std::stoi(operand1) + std::stoi(operand2);
                    return 1;
                    break;
                case '-':
                    result = std::stoi(operand1) - std::stoi(operand2);
                    return 1;
                    break;
                case '*':
                    result = std::stoi(operand1) * std::stoi(operand2);
                    return 1;
                    break;
                case '/':
                    if (std::stoi(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    result = std::stoi(operand1) / std::stoi(operand2);
                    return 1;
                    break;
                default:
                    break;
                }
            }
            else if (is_operand(operand1) == 1 && is_integer(operand2) == 1 && is_operator(op) == 1)
            {
                switch (op[0])
                {
                case '+':
                    result = var_dict.at(operand1) + std::stoi(operand2);
                    return 1;
                    break;
                case '-':
                    result = var_dict.at(operand1) - std::stoi(operand2);
                    return 1;
                    break;
                case '*':
                    result = var_dict.at(operand1) * std::stoi(operand2);
                    return 1;
                    break;
                case '/':
                    if (std::stoi(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    result = var_dict.at(operand1) / std::stoi(operand2);
                    return 1;
                    break;
                case '=':
                    if (var_exist(operand1) == 0)
                    {
                        var_dict.insert(std::pair<std::string, int>(operand1, std::stoi(operand2)));
                    } else {
                        var_dict.at(operand1) = std::stoi(operand2);
                    }
                    result = var_dict.at(operand1);
                    return 1;
                    break;
                default:
                    break;
                }
            } 
            else if (is_operand(operand1) == 1 && is_operand(operand2) == 1 && is_operator(op) == 1)
            {
                switch (op[0])
                {
                case '+':
                    result = var_dict.at(operand1) + var_dict.at(operand2);
                    return 1;
                    break;
                case '-':
                    result = var_dict.at(operand1) - var_dict.at(operand2);
                    return 1;
                    break;
                case '*':
                    result = var_dict.at(operand1) * var_dict.at(operand2);
                    return 1;
                    break;
                case '/':
                    if (var_dict.at(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    result = var_dict.at(operand1) / var_dict.at(operand2);
                    return 1;
                    break;
                case '=':
                    if (var_exist(operand1) == 0)
                    {
                        var_dict.insert(std::pair<std::string, int>(operand1, var_dict.at(operand2)));
                    } else {
                        var_dict.at(operand1) = var_dict.at(operand2);
                    }
                    result = var_dict.at(operand1);
                    return 1;
                    break;
                default:
                    break;
                }
            }
            break;
            
        }
        case 5:
        {
            std::string var = tokens.at(0);
            std::string op1 = tokens.at(1);
            std::string operand1 = tokens.at(2);
            std::string op2 = tokens.at(3);
            std::string operand2 = tokens.at(4);
            int temp_res = 0;

            if (is_integer(operand1) == 1 && is_integer(operand2) == 1 && is_operator(op2) == 1)
            {
                switch (op2[0])
                {
                case '+':
                    temp_res = std::stoi(operand1) + std::stoi(operand2);
                    break;
                case '-':
                    temp_res = std::stoi(operand1) - std::stoi(operand2);
                    break;
                case '*':
                    temp_res = std::stoi(operand1) * std::stoi(operand2);
                    break;
                case '/':
                    if (std::stoi(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    temp_res = std::stoi(operand1) / std::stoi(operand2);
                    break;
                default:
                    break;
                }
            }
            else if (is_operand(operand1) == 1 && is_integer(operand2) == 1 && is_operator(op2) == 1)
            {
                switch (op2[0])
                {
                case '+':
                    temp_res = var_dict.at(operand1) + std::stoi(operand2);
                    break;
                case '-':
                    temp_res = var_dict.at(operand1) - std::stoi(operand2);
                    break;
                case '*':
                    temp_res = var_dict.at(operand1) * std::stoi(operand2);
                    break;
                case '/':
                    if (std::stoi(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    temp_res = var_dict.at(operand1) / std::stoi(operand2);
                    break;
                default:
                    break;
                }
            } 
            else if (is_operand(operand1) == 1 && is_operand(operand2) == 1 && is_operator(op2) == 1)
            {
                switch (op2[0])
                {
                case '+':
                    temp_res = var_dict.at(operand1) + var_dict.at(operand2);
                    break;
                case '-':
                    temp_res = var_dict.at(operand1) - var_dict.at(operand2);
                    break;
                case '*':
                    temp_res = var_dict.at(operand1) * var_dict.at(operand2);
                    break;
                case '/':
                    if (var_dict.at(operand2) == 0) {
                        std::cout << "Expression is invalid (attempt to divide by 0)." << std::endl;
                        return 0;
                    }
                    temp_res = var_dict.at(operand1) / var_dict.at(operand2);
                    break;
                default:
                    break;
                }
            }
            

            if (op1[0] != '=') {
                // format is not correct
                std::cout << "Expression is invalid" << std::endl;
                return 0;
            }

            if (var_exist(var) == 0) {
                var_dict.insert(std::pair<std::string, int>(var, temp_res));
            }
            else {
                var_dict.at(var) = temp_res;
            }
            result = temp_res;
            return 1;
            break;
        }
        default:
        {
            return 0;
        }
    }
    return 0;
}

extern "C" std::vector<std::string> Calc::tokenize(const std::string &expr) {
    std::vector<std::string> vec;
    std::stringstream s(expr);

    std::string tok;
    while (s >> tok) {
        vec.push_back(tok);
    }

    return vec;
}

extern "C" int Calc::is_variable(std::string operand) {

    for (std::string::iterator it = operand.begin(); it != operand.end(); it++)
    {
        if (isalpha(*it) == 0)
        {
            return 0;       // operand is not valid variable
        }
        
    }
    
    return 1;       // operand is variable
}

extern "C" int Calc::is_integer(std::string operand) {
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

extern "C" int Calc::is_operand(std::string operand) {
    if (is_integer(operand) == 1 || is_variable(operand) == 1)
    {
        return 1;       // is valid operand
    }
    
    return 0;       // is not valid operand
}

extern "C" int Calc::is_operator(std::string op) {
    std::vector<std::string> operators {"+", "-", "*", "/", "="};
    if (std::find(operators.begin(), operators.end(), op) != operators.end())
    {
        return 1;       // is valid operator
    }
    
    return 0;       // is not valid operator
}