#include "ftokenize.h"

bool FTokenizer::get_new_block() {
    const bool debug = false;

    if(debug) cout << endl << "----- New Block ---------------------[" << MAX_BUFFER - 1 << "] bytes" << endl;
    
    char _buffer[MAX_BUFFER];
    _f.read(_buffer, MAX_BUFFER - 1);

    if(debug) cout << endl << "*** get_new_block() : printing _buffer..." << endl;
    if (debug) {
        for (int i = 0; i < _f.gcount(); i++) {
            if (i != 0 && i % 49 == 0) // new line so printout isn't too long
                cout << endl;
            cout << _buffer[i];
        } cout << endl;
    }

    if (_f.gcount() == 0)
        return false;
    _buffer[_f.gcount()] = '\0';

    _blockPos = 0;
    _pos += _f.gcount();
    _stk.set_string(_buffer);
    return true;
}

FTokenizer::FTokenizer(char* fname) : _pos(0), _blockPos(0) {
    _f.open(fname);
    if (_f.fail()) {
        std::cout << endl << "FTokenizer() : Could not open file " << fname << endl;
        exit(1);
    }
    _more = get_new_block();
}

/**
* @returns A new token from the file; default token if FTokenizer#more() == false;
*/
string FTokenizer::next_token() {
    const bool debug = false;
    string t;
    _stk >> t;
    if (!_stk.more()) {
        if (debug) cout << "*** next_token() : _stk is empty!" << endl << endl;
        _more = get_new_block();
        _stk >> t;
    }
    _blockPos += t.size();
    return t;
}

/**
* @returns Whether or not there are more tokens to be tokenized in the file
*/
bool FTokenizer::more() {
    return _more;
}

/**
* @returns The position in the file
*/
int FTokenizer::pos() {
    return _pos;
}

/**
* @returns The position in the current block extracted from the file
*/
int FTokenizer::block_pos() {
    return _blockPos;
}