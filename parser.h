#ifndef PARSER_H
#define PARSER_H
#include "tokenizer.h"
#include"statement.h"
#include"exp.h"
class parser
{
private:
    exp *exp_tree;
    state_t kind;
public:
    parser();
    parser(statement *newline);
};

#endif // PARSER_H
