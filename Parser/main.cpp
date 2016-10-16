#include <iostream>

#include "parser.h"

using namespace std;

int main() {
    parser p;
    cout << "Hello, World!" << endl;
    cout << p.getPixelStream("C:\\Users\\Kevin\\Documents\\College\\FYP\\images\\test_1.tif") << endl;
    return 0;
}