//
// Created by Kevin on 14/10/2016.
//

#include"parser.h"

Pixel::Pixel(int r, int g, int b){
    this->r = r; this->g = g; this->b = b;
}

int Pixel::getR(){ return this->r; }
int Pixel::getG(){ return this->g; }
int Pixel::getB(){ return this->b; }

Sampler::Sampler(const char* TIFF_filename, const char* RAW_filename){
    
}

Sampler::TIFFparser(const char* filename){
    TIFF* filestream = TIFFOpen(filename, "r");
    
    if(filestream){
        uint32 w, h;
        size_t npixels;
        uint32* raster;

        TIFFGetField(filestream, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(filestream, TIFFTAG_IMAGELENGTH, &h);
        npixels = w * h;
        this->tiff_width = (int) w;
        this->tiff_height = (int) h;
        raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        if (raster != NULL) {
            TIFFReadRGBAImageOriented(filestream, w, h, raster, ORIENTATION_TOPLEFT, 0);
        _TIFFfree(raster);
        }
        this->tiff_pixels = (Pixel *) malloc(npixels*sizeof(Pixel));
        for(int i = 0; i < npixels; i++){
            this->tiff_pixels[i] = Pixel(
                    TIFFGetR(raster[i]),
                    TIFFGetG(raster[i]),
                    TIFFGetB(raster[i]));
        }
    }
}

int Sampler::TIFFgetWidth(){ return this->tiff_width; }
int Sampler::TIFFgetHeight(){ return this->tiff_height; }
Pixel* Sampler::TIFFgetPixels(){ return this->tiff_pixels; }

Sampler::RAWparser(const char* filename){
    LibRaw processor;
    processor.open_file(filename);
    processor.unpack();
    processor.raw2image();
    this->raw_width = (int) processor.imgdata.sizes.width;
    this->raw_height = (int) processor.imgdata.sizes.height;
    int npixels = this->raw_width * this->raw_height;
    this->raw_pixels = (Pixel *) malloc(npixels * sizeof(Pixel));
    for(int i = 0; i < npixels; i++){
        this->raw_pixels[i] = Pixel(
                processor.imgdata.image[i][0],
                processor.imgdata.image[i][1],
                processor.imgdata.image[i][2]);
    }
    processor.recycle();
}

int Sampler::RAWgetHeight(){ return this->raw_height; }
int Sampler::RAWgetWidth(){ return this->raw_width; }
Pixel* Sampler::RAWgetPixels(){ return this->raw_pixels; }