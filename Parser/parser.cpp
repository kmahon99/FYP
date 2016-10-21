//
// Created by Kevin on 14/10/2016.
//

#include"parser.h"

using namespace std;

Pixel::Pixel(){};

Pixel::Pixel(int r, int g, int b){
    this->r = r; this->g = g; this->b = b;
}

int Pixel::getR(){ return this->r; }
int Pixel::getG(){ return this->g; }
int Pixel::getB(){ return this->b; }

Sampler::Sampler(const char* img_filename, 
        const char* annotation, 
        int patch_size){
    ClassParser classes("/home/kev/Documents/FYP/FYP/Parser/classes.txt");
    
    TIFFparser image(img_filename);
    TIFFparser ann(annotation);
    if(image.getWidth() == ann.getWidth() 
            && image.getHeight() == ann.getHeight()){
        Pixel** image_pixels = image.getPixelStream();
        Pixel** ann_pixels = ann.getPixelStream();
        
        for(int i = 0; i < image.getWidth(); i++){
            for(int j = 0; j < image.getHeight(); j++){
                if(ann_pixels[i][j].getR() != 255 &&
                    ann_pixels[i][j].getG() != 255 &&
                    ann_pixels[i][j].getB() != 255){
                    cout << classes.getClassname(ann_pixels[i][j]) << endl;
                }
            }
        }
    }
    else{ cout << "Image dimensions don't match the annotation dimensions!" << endl; }
}

TIFFparser::TIFFparser(const char* filename){
    TIFF* filestream = TIFFOpen(filename, "r");
    
    if(filestream){
        uint32 w, h;
        size_t npixels;
        uint32* raster;

        TIFFGetField(filestream, TIFFTAG_IMAGEWIDTH, &w);
        TIFFGetField(filestream, TIFFTAG_IMAGELENGTH, &h);
        
        npixels = w * h;
        this->width = (int) w;
        this->height = (int) h;
        raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
        
        if (raster != NULL) {
            TIFFReadRGBAImageOriented(filestream, w, h, raster, ORIENTATION_TOPLEFT, 0);
        _TIFFfree(raster);
        }
        
        //Put raster into 2D array
        this->pixels = (Pixel **) malloc(height * sizeof(Pixel *));
        for(int i = 0; i < height; i++){
            this->pixels[i] = (Pixel *) malloc(width * sizeof(Pixel));
            for(int j = 0; j < width; j++){
                this->pixels[i][j] = Pixel(
                        TIFFGetR(raster[(i*width)+j]),
                        TIFFGetG(raster[(i*width)+j]),
                        TIFFGetB(raster[(i*width)+j]));
            }
        }
    }
}

int TIFFparser::getWidth(){ return this->width; }
int TIFFparser::getHeight(){ return this->height; }
Pixel** TIFFparser::getPixelStream(){ return this->pixels; }

Landclass::Landclass(string name, Pixel pixel){
    this->name = name;
    this->pixel = pixel;
}

string Landclass::getName(){ return this->name; }

bool Landclass::hasPixel(Pixel pixel){
    if(this->pixel.getR() == pixel.getR() &&
            this->pixel.getG() == pixel.getG() &&
            this->pixel.getB() == pixel.getB()){
        return true;
    }else{ return false; }
}

ClassParser::ClassParser(const char* filename){
    ifstream file;
    int linecount = 0;
    file.open(filename);
    if(file.is_open()){
        string line, classname, pixel;
        while(getline(file,line)){
            if(linecount % 2 == 0){
                classname = line;
            }else{
                stringstream ss(line);
                vector<int> result;
                while(ss.good()){
                    string sub;
                    getline(ss,sub,',');
                    result.push_back(atoi(sub.c_str()));
                }
                if(result.size() == 3){
                    Pixel p(result[0],result[1],result[2]);
                    classes.push_back(Landclass(classname,p));
                }
                else{ cout << "Badly formatted class line found!" << endl; }
            }
            linecount++;
        }
        if(linecount % 2 != 0){ 
            cout << "Classes file doesn't contain proper name-value pairs" << endl; 
        }
        this->nclasses = linecount/2;
    }else{ cout << "Classes file not found!" << endl; }
    file.close();
}

string ClassParser::getClassname(Pixel pixel){
    for(int i = 0; i < this->classes.size(); i++){
        if(this->classes[i].hasPixel(pixel)){ return this->classes[i].getName(); }
    }
    return "NONE";
}