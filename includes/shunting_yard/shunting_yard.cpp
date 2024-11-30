#include "shunting_yard.h"

ShuntingYard::ShuntingYard(Queue<Token*>& infix) {
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
                /// Utilized for giving AND precedence over OR (AND is popped from op_stack if OR would be on top, but not vice versa)
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
                if (popped->type() == LPAREN) break;
                output_queue.push(popped);
            }
            break;
        default:
            break;
        }
        if (debug) {
            cout << "ShuntingYard() : op_stack = Head-> ";
            Stack<Token*>::Iterator it = op_stack.begin();
            while (it != op_stack.end()) {
                cout << **it << " ";
                it++;
            } cout << "|||" << endl;
        }
    }

    while (op_stack.size() > 0) {
        Token* popped = op_stack.pop();
        // assert(popped->token_str() != "("); ///< Should be impossible that an invalid infix is passed, but just in case for debugging
        output_queue.push(popped);
    }

    if (debug) {
        cout << "ShuntingYard() : output_queue = ";
        output_queue.print_pointers();
    }

    this->_postfix = output_queue;
}

const Queue<Token*>& ShuntingYard::postfix() {
    return this->_postfix;
}