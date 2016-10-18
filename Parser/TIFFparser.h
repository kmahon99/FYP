//
// Created by Kevin on 14/10/2016.
//

#ifndef TIFF_PARSER_H
#define TIFF_PARSER_H

#include <string>
#include <tiffio.h>

using namespace std;

class TIFFparser {
public:
    TIFFparser(const char* filename);
    TIFF* getFileStream();
    uint32* getPixelStream();
private:
    TIFF* filestream;
    uint32* raster;
};

#endif //TIFF_PARSER_H
