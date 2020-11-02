#include <iostream>
#include "Python.h"
#include "mysql.h"
#include "config.hpp"

using namespace std;

PyObject* get(){
    cout << "Python" << endl;
    return Py_None;
}
int main(int args, char* argv[]){
    cout << "你好" << endl;
#ifdef USER_DEFINE_CONFIG
    cout << "true" << endl;
#else
    cout << "false" << endl;
    get();
#endif
}