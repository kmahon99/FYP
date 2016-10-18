//
// Created by Kevin on 14/10/2016.
//

#include "TIFFparser.h"

#include<sstream>
#include<iostream>
#include<bitset>

TIFFparser::TIFFparser(const char* filename){
    this->filestream = TIFFOpen(filename, "r");
    
    if(this->filestream){
        uint32 w, h;
        size_t npixels;
        uint32* raster;

        TIFFGetField(this->filestream, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(this->filestream, TIFFTAG_IMAGELENGTH, &h);
        npixels = w * h;
        raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) {
            TIFFReadRGBAImageOriented(this->filestream, w, h, raster, ORIENTATION_TOPLEFT, 0);
        _TIFFfree(raster);
        }
    }
}

TIFF* TIFFparser::getFileStream(){
    return this->filestream;
}

uint32* TIFFparser::getPixelStream(){
    return this->raster;
}
