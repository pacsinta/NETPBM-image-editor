#include "image.h"
#include "debugmalloc.h"

void allocImg(Img* img) {
    img->pixels = (Pixel**)malloc(img->height * sizeof(Pixel));
    for (int i = 0; i < img->height; i++)
    {
        img->pixels[i] = (Pixel*)malloc(img->width * sizeof(Pixel));
    }
}

void freeImg(Img* img){
    //kép törlése a memóriából
    for (int i = 0; i < img->height; i++)
    {
        free(img->pixels[i]);
    }
    free(img->pixels);
}