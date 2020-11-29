#ifndef IMAGEE_H_INCLUDED
#define IMAGEE_H_INCLUDED

#include "image.h"

void fenyerovaltoztatas(Img* img, int fenyero);
void contrastvaltoztatas(Img* img, int contrast);
void grayscale(Img* img);
void convertToBinaryImage(Img* img);
void blur(Img* img, int count);

#endif