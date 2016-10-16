//
// Created by Kevin on 14/10/2016.
//

#include "parser.h"

#include "libtiff/include/tiffio.h"

string parser::getPixelStream(const char* filename){

    TIFF *tif = TIFFOpen(filename, "r");

    unsigned long width, height;

    string str = "test";

    return str;
}
