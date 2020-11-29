#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

static const unsigned int MaxLineSize = 70;

enum netpbmType {
    PPM_ASCII,
    PPM_BINARY,
    PBM_ASCII,
    PBM_BINARY,
    PGM_ASCII,
    PGM_BINARY
};

typedef struct Pixel {
    int r, g, b;
} Pixel;

typedef struct Img {
    enum netpbmType type;
    int width, height;
    int maxValue;
    Pixel** pixels;
} Img;

void allocImg(Img* img);
void freeImg(Img* img);
#endif