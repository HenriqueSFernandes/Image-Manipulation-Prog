#include "Script.hpp"
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cout << "Usage: " << argv[0] << " script1 script2 ..." << endl;
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        cout << "==> Running script: " << argv[i] << endl;
        prog::Script script(argv[i]);
        script.run();
        cout << endl;
    }
    cout << "==> Done :) " << endl;
    return 0;
}