#include <iostream>
#include <iomanip>


using namespace std;

int* _test() {
    int* _ptr = new int(100);
    cout << "_test() : _ptr = " << _ptr << " | *_ptr = " << *_ptr << endl;
    return _ptr;
}

int main(int argv, char** argc) {
    cout << "\n\n"
         << endl;

    int* pointer = _test();
    cout << " main() : _ptr = " << pointer << " | *_ptr = " << *pointer << endl;


    delete pointer;
    pointer = nullptr;

    cout << "\n\n\n=====================" << endl;
    return 0;
}
