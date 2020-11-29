#ifndef IMAGER_H_INCLUDED
#define IMAGER_H_INCLUDED

#include "image.h"
#include <stdio.h>

void printImgData(Img* img, FILE* fp);
void mentes(Img* img, char* name);
int readImg(char* img_location, Img* img);

#endif