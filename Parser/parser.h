//
// Created by Kevin on 14/10/2016.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <tiffio.h>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

class Pixel{
public:
    Pixel();
    Pixel(int r, int g, int b);
    int getR();
    int getG();
    int getB();
    string toString();
private:
    int r, g, b;
};

class TIFFparser{
public:
    TIFFparser(const char* filename);
    Pixel** getPixelStream();
    int getWidth();
    int getHeight();
private:
    int width, height;
    Pixel** pixels;
};

class Sampler{
public:
    Sampler(const char* img_filename, const char* annotation, int patch_size);
private:
    string getExample(Pixel** stream, int i, int j, int patch_size, string classname, int width, int height);
    int nexamples, insize, nclasses, examplenum;
};

class Landclass{
public:
    Landclass();
    Landclass(string name, Pixel pixel);
    string getName();
    bool hasPixel(Pixel pixel);
private:
    string name;
    Pixel pixel;
};

class ClassParser{
public:
    ClassParser(const char* filename);
    string getClassname(Pixel pixel);
    int getNumClasses();
private:
    vector<Landclass> classes;
    int nclasses;
};

#endif //PARSER_H
