//
// Created by Kevin on 14/10/2016.
//

#include"parser.h"

using namespace std;

Pixel::Pixel(){};

Pixel::Pixel(int r, int g, int b){
    this->r = r; this->g = g; this->b = b;
}

string Pixel::toString(){
    stringstream ss;
    if(this->r != 0) ss << 255/this->r << " ";
    else ss << this->r << " ";
    if(this->g != 0) ss << 255/this->g << " ";
    else ss << this->g << " ";
    if(this->b != 0) ss << 255/this->b << endl;
    else ss << this->b;
    return ss.str();
}

int Pixel::getR(){ return this->r; }
int Pixel::getG(){ return this->g; }
int Pixel::getB(){ return this->b; }

Sampler::Sampler(const char* img_filename, 
        const char* annotation, 
        int patch_size){
    
    this->examplenum = this->nexamples = 0;
    TIFFparser image(img_filename);
    TIFFparser ann(annotation);
    stringstream pixel_stream;
    vector<string> examples;
    ClassParser classes("/home/kev/Documents/FYP/FYP/Parser/classes.txt");
    
    ofstream file;
    file.open("OUTPUT.txt");
    
    Pixel** img_pixels = image.getPixelStream();
    Pixel** ann_pixels = ann.getPixelStream();
    
    if(image.getWidth() == ann.getWidth() && image.getHeight() == ann.getHeight()){
        for(int i = 0; i < ann.getWidth(); i++){
            for(int j = 0; j < ann.getHeight(); j++){
                this->nexamples++;
                string classname = classes.getClassname(ann_pixels[i][j]);
                examples.push_back(this->getExample(img_pixels, i, j, patch_size, classname, image.getWidth(), image.getHeight()));
            }
        }
    }else { cout << "Image and annotation dimensions don't match!" << endl; } 
    
    file << this->nexamples << endl;
    file << patch_size*patch_size*3 << this->nclasses << endl;
    for(int i = 0; i < examples.size(); i++){
        file << examples.at(i);
    }
    
    file.close();
}

string Sampler::getExample(Pixel** stream, int i, int j, int patch_size, string classname, int width, int height){
    stringstream ss;
    ss << "example_" << this->examplenum++ << endl << 1 << endl;
    for(int k = i - patch_size + 1; k < i + patch_size; k++){
        if(k >= 0 and k < width){
            for(int l = j - patch_size + 1; l < j + patch_size; l++){
                if(l >= 0 and l < height){
                    ss << stream[k][l].toString() << " ";
                }
            }
        }
    }   
    ss << endl << classname << endl << endl;  
    return ss.str();
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

int ClassParser::getNumClasses(){ return this->nclasses; }