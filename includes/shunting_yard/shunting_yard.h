#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "../token/token.h"

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
    ShuntingYard(Queue<Token*>& infix) {
        const bool debug = false;

        Queue<Token*> output_queue;
        Stack<Token*> op_stack;

        for (Queue<Token*>::Iterator it = infix.begin(); it != infix.end(); it++) {
            TokenType _type = (*it)->type();

            if (debug) cout << "ShuntingYard() : _token = " << (*it)->token_str() << " | _type = " << _type << endl;

            switch (_type) {
            case STRING:
                output_queue.push(*it);
                break;
            case RELAT:
                op_stack.push(*it);
                break;
            case LOGIC:
                if (op_stack.top()->type() >= _type)
                    output_queue.push(op_stack.pop());
                op_stack.push(*it);
                break;
            case RPAREN:
                if (op_stack.top()->type() >= _type)
                    output_queue.push(op_stack.pop());
                break;
            default:
                break;
            }
        }

        while (op_stack.size() > 0)
            output_queue.push(op_stack.pop());

        if (debug) {
            cout << "infix_to_postfix() : output_queue = ";
            output_queue.print_pointers();
        }

        this->_postfix = output_queue;
    }

    Queue<Token*> postfix() {
        return _postfix;
    }

};

#endif