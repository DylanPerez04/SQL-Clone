#include "state_machine_functions.h"
#include <string>

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS]) {
    for (int i = 0; i < MAX_ROWS; i++)
        for (int j = 0; j < MAX_COLUMNS; j++)
            _table[i][j] = -1;
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state) {
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state) {
    _table[state][0] = 0;
}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state) {
    if (state < 0) return false;
    return _table[state][0] == 1;
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state) {
    for (int i = from; i <= to; i++)
        _table[row][i] = state;
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state) {
    for (int i = 0; columns[i] != '\0'; i++)
        _table[row][(int)columns[i]] = state;
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state) {
    _table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS]) {
    const int rows_used = 45;
    const int col_min = 0, col_max = 15;
    const int width = 6;
    
    std::string label;
    for (int i = -1; i < rows_used; i++) {
        if (i >= 0) {
            label = "[" + std::to_string(i) + "]";
            std::cout << std::setw(width) << label;
        }
        else
            std::cout << std::setw(width) << "[#]";

        for (int j = col_min; j < col_max; j++) {
            if (i == -1) { // print ascii characters
                label = "| " + std::to_string(j) + " |";
                std::cout << std::setw(width / 2) << "" << label;
            }
            else {
                label = std::to_string(_table[i][j]);
                std::cout << std::setw(width / 2) << "" << std::setw(3) << label << "  ";
            }
        } 
        std::cout << std::endl;
    }

}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos) {

    for (int i = 0; s[i] != '\0'; i++)
        std::cout << s[i];
    std::cout << "  pos: " << _pos << std::endl;
    for (int i = 0; i < _pos; i++)
        std::cout << " ";

    std::cout << "^" << std::endl;
}