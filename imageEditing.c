#include "imageEditing.h"
#include "debugmalloc.h"

//Ellenõrzi, hogy egy pixel ne legyen kisebb 0-nál és ne legyen nagyobb a maximum megengedett értéknél
int tulcsordulasVedelem(int x, int maxVal) {
    if (x < 0) {
        return 0;
    }
    else if (x > maxVal) {
        return maxVal;
    }
    return x;
}

void fenyerovaltoztatas(Img* img, int fenyero) {
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            img->pixels[i][j].r = tulcsordulasVedelem(img->pixels[i][j].r + fenyero, img->maxValue);
            img->pixels[i][j].g = tulcsordulasVedelem(img->pixels[i][j].g + fenyero, img->maxValue);
            img->pixels[i][j].b = tulcsordulasVedelem(img->pixels[i][j].b + fenyero, img->maxValue);
        }
    }
}

void contrastvaltoztatas(Img* img, int contrast) {
    if(contrast>300 || contrast<-300){
        printf("Tul nagy/kicsi kontrasztérték");
        return;
    }

    double dc = contrast;

    if(dc == 0){
        dc = 1;
    }else{
        dc = 1 + dc/300.0;
    }

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            img->pixels[i][j].r = tulcsordulasVedelem(dc * ((double)img->pixels[i][j].r - (double)img->maxValue/2.0) + (double)img->maxValue/2, img->maxValue);
            img->pixels[i][j].g = tulcsordulasVedelem(dc * ((double)img->pixels[i][j].g - (double)img->maxValue/2.0) + (double)img->maxValue/2, img->maxValue);
            img->pixels[i][j].b = tulcsordulasVedelem(dc * ((double)img->pixels[i][j].b - (double)img->maxValue/2.0) + (double)img->maxValue/2, img->maxValue);
        }
    }
}

void grayscale(Img* img) {
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int gray = img->pixels[i][j].r + img->pixels[i][j].g + img->pixels[i][j].b;
            gray = gray / 3;
            img->pixels[i][j].r = gray;
            img->pixels[i][j].g = gray;
            img->pixels[i][j].b = gray;
        }
    }
}

void convertToBinaryImage(Img* img) {
    if(img->type == PBM_BINARY || img->type == PBM_ASCII) return;
    grayscale(img);
    for (int i = 0; i < img->height; i++)
    {
        for (int x = 0; x < img->width; x++)
        {
            if (img->pixels[i][x].r >= (img->maxValue / 2)) {
                img->pixels[i][x].r = img->maxValue;
                img->pixels[i][x].g = img->maxValue;
                img->pixels[i][x].b = img->maxValue;
            }
            else {
                img->pixels[i][x].r = 0;
                img->pixels[i][x].g = 0;
                img->pixels[i][x].b = 0;
            }
        }
    }
    img->type = PBM_ASCII;
}

void blur(Img* img, int count) {

    const int radius = 1;

    for (int c = 0; c < count; c++)
    {
        for (int i = 0; i < img->height; i++)
        {
            for (int x = 0; x < img->width; x++)
            {
                int sum[3];  //kulon kell tarolni az rgb szineket
                sum[0] = 0;
                sum[1] = 0;
                sum[2] = 0;
                if (img->pixels[i][x].g == 15) {
                    int t = 0;
                }
                for (int z = 0; z < radius * 2 + 1; z++)
                {
                    for (int a = 0; a < radius * 2 + 1; a++)
                    {
                        //Fent van tulcsordulás
                        if (i - radius + z < 0) {
                            //Balra van túlcsordulás
                            if (x - radius + a < 0) {
                                sum[0] += img->pixels[0][0].r;
                                sum[1] += img->pixels[0][0].g;
                                sum[2] += img->pixels[0][0].b;
                            }
                            //Jobbra van túlcsordulás
                            else if (x - radius + a >= img->width) {
                                sum[0] += img->pixels[0][img->width - 1].r;
                                sum[1] += img->pixels[0][img->width - 1].g;
                                sum[2] += img->pixels[0][img->width - 1].b;
                            }
                            //Oldalt nincs túlcsordulás
                            else {
                                sum[0] += img->pixels[0][x - radius + a].r;
                                sum[1] += img->pixels[0][x - radius + a].g;
                                sum[2] += img->pixels[0][x - radius + a].b;
                            }
                        }
                        //Lent van túlcsordulás
                        else if (i - radius + z >= img->height) {
                            //Balra van túlcsordulás
                            if (x - radius + a < 0) {
                                sum[0] += img->pixels[img->height - 1][0].r;
                                sum[1] += img->pixels[img->height - 1][0].g;
                                sum[2] += img->pixels[img->height - 1][0].b;
                            }
                            //Jobbra van túlcsordulás
                            else if (x - radius + a >= img->width) {
                                sum[0] += img->pixels[img->height - 1][img->width - 1].r;
                                sum[1] += img->pixels[img->height - 1][img->width - 1].g;
                                sum[2] += img->pixels[img->height - 1][img->width - 1].b;
                            }
                            //Oldalt nincs túlcsordulás
                            else {
                                sum[0] += img->pixels[img->height - 1][x - radius + a].r;
                                sum[1] += img->pixels[img->height - 1][x - radius + a].g;
                                sum[2] += img->pixels[img->height - 1][x - radius + a].b;
                            }
                        }
                        //Csak balra van túlcsordulás
                        else if (x - radius + a < 0) {
                            sum[0] += img->pixels[i - radius + z][0].r;
                            sum[1] += img->pixels[i - radius + z][0].g;
                            sum[2] += img->pixels[i - radius + z][0].b;
                        }
                        //Csak jobbra van túlcsordulás
                        else if (x - radius + a >= img->width) {
                            sum[0] += img->pixels[i - radius + z][img->width - 1].r;
                            sum[1] += img->pixels[i - radius + z][img->width - 1].g;
                            sum[2] += img->pixels[i - radius + z][img->width - 1].b;
                        }
                        //Nincs túlcsordulás
                        else {
                            sum[0] += img->pixels[i - radius + z][x - radius + a].r;
                            sum[1] += img->pixels[i - radius + z][x - radius + a].g;
                            sum[2] += img->pixels[i - radius + z][x - radius + a].b;
                        }
                    }
                }
                img->pixels[i][x].r = sum[0] / 9;
                img->pixels[i][x].g = sum[1] / 9;
                img->pixels[i][x].b = sum[2] / 9;
            }
        }
    }

}