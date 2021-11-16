#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>

/*
 * Note: you should NOT need to modify anything in this header file.
 */

/* Forward declaration of the struct Calc data type. */
struct Calc;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * You should implement these functions in calc.c or calc.cpp
 * (according to whether you are programming in C or C++.)
 * If you define them in C++, make sure they have extern "C"
 * linkage.
 */
struct Calc *calc_create(void);
void calc_destroy(struct Calc *calc);
int calc_eval(struct Calc *calc, const char *expr, int *result);
std::vector<std::string> tokenize(const std::string &expr);
int is_variable(std::string operand);
int is_integer(std::string operand);
int var_exist(std::string var);

#ifdef __cplusplus
}
#endif

#endif /* CALC_H */
