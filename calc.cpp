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
private:
    // private member functions
    
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

int Calc::evalExpr(const std::string &expr, int &result) {
    std::vector<std::string> tokens = tokenize(expr);
    int num_tokens = tokens.size();
    if (num_tokens > 5 || num_tokens == 4 || num_tokens == 2 || num_tokens < 1)
    {
        return 0;
    }
    
}

std::vector<std::string> tokenize(const std::string &expr) {
    std::vector<std::string> vec;
    std::stringstream s(expr);

    std::string tok;
    while (s >> tok) {
        vec.push_back(tok);
    }

    return vec;
}