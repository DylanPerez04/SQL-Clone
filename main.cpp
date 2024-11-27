#include <iostream>
#include <iomanip>


using namespace std;

int* _test1() {
    int* _ptr = new int(100);
    cout << "_test1() : _ptr = " << _ptr << " | *_ptr = " << *_ptr << endl;
    return _ptr;
}

int _test2() {
    int num = 100;
    cout << "_test2() : num = " << num << " | &num = " <<  &num << endl;
    return num;
}

int* _test3() {
    int num = 100;
    cout << "_test3() : num = " << num << " | &num = " << &num << endl;
    return &num;
}

int main(int argv, char** argc) {
    cout << "\n\n"
         << endl;

    int* pointer = _test1();
    cout << " main() : _ptr = " << pointer << " | *_ptr = " << *pointer << endl;

    int num = _test2();
    cout << "main() : num = " << num << " | &num = " << &num << endl;

    int* num2 = _test3();
    cout << "main() : *num2 = " << *num2 << "| num2 = " << num2 << endl;


    delete pointer;
    pointer = nullptr;

    cout << "\n\n\n=====================" << endl;
    return 0;
}
