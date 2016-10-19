//
// Created by Kevin on 14/10/2016.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdlib.h>
#include <tiffio.h>
#include <libraw/libraw.h>

using namespace std;

class Pixel{
public:
    Pixel(int r, int g, int b);
    int getR();
    int getG();
    int getB();
private:
    int r, g, b;
};

class Sampler{
public:
    Sampler(const char* TIFF_filename, const char* RAW_filename);
private:
    TIFFparser(const char* filename);
    int TIFFgetWidth();
    int TIFFgetHeight();
    Pixel* TIFFgetPixels();
    RAWparser(const char* filename);
    int RAWgetWidth();
    int RAWgetHeight();
    Pixel* RAWgetPixels();
    int tiff_width, tiff_height, raw_width, raw_height;
    Pixel* tiff_pixels, raw_pixels;
};

#endif //PARSER_H
