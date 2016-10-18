#include <iostream>

#include "TIFFparser.h"

using namespace std;

int main(int argc, char* argv[]) {
    TIFFparser p("/home/kev/Documents/FYP/FYP/images/test_1.TIF");
    cout << "Hello, World!" << endl;
    cout << p.getPixelStream() << endl;
    return 0;
}