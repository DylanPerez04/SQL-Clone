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

            if (debug) cout << "ShuntingYard() : _token = " << (*it)->token_str() << " | TokenType _type = " << _type << endl;

            switch (_type) {
            case STRING:
                output_queue.push(*it);
                break;
            case RELAT:
                op_stack.push(*it);
                break;
            case LOGIC:
                while (!op_stack.empty() && op_stack.top()->type() >= _type) {
                    /// Utilized for giving AND precedence over OR
                    if (Operator::get_operator((*it)->token_str()) > Operator::get_operator(op_stack.top()->token_str()))
                        break;
                    
                    output_queue.push(op_stack.pop());
                }
                op_stack.push(*it);
                break;
            case LPAREN: // TODO : See if I can have this case be next to case RELAT
                op_stack.push(*it);
                break;
            case RPAREN:
                while (!op_stack.empty() && op_stack.top()->type() >= _type) {
                    Token* popped = op_stack.pop();
                    if(popped->type() != LPAREN) output_queue.push(popped); ///< Ensure ( isn't added to output queue
                }
                break;
            default:
                break;
            }
        }

        while (op_stack.size() > 0)
            output_queue.push(op_stack.pop());

        if (debug) {
            cout << "ShuntingYard() : output_queue = ";
            output_queue.print_pointers();
        }

        this->_postfix = output_queue;
    }

    Queue<Token*> postfix() {
        return _postfix;
    }

};

#endif