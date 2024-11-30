#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/child_tokens.h"

struct ShuntingYard {
private:
    Queue<Token*> _postfix;
public:

    /*
    * Takes an in-fix expression and converts it into post-fix
    */
    // operator stack and output queue
    // never sit on someone whose not smaller than you. (pop stack and insert into queue)
    // Throw parenthesis away 

    /*
    * infix = "(", "age", "<", "17", "or", "age", ">", "20", ")", "and", "lname", "=", "Jackson"
    * postfix = "age", "17", "<", "age", "20", ">", "or", "lname", "Jackson", "=", "and"
    */
    ShuntingYard(Queue<Token*>& infix);

    Queue<Token*>& postfix();

};

#endif