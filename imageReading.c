#include "imageReading.h"
#include <stdio.h>
#include <string.h>
#include "debugmalloc.h"

//Kiírja a paraméterként megadott kép pixeleinek rgb értékét egy paraméterként megadott file-ba. (lehet az stdout-is)
void printImgData(Img* img, FILE* fp) {
    if(img->width < MaxLineSize * 7){
        for (int i = 0; i < img->height; i++)
        {
            for (int j = 0; j < img->width; j++)
            {
                fprintf(fp, " ");

                if(img->type == PBM_BINARY || img->type == PBM_ASCII){
                    //.pbm esetén 0 a fehér, és 1 a fekete
                    fprintf(fp, "%d", img->pixels[i][j].r == 0 ? 1 : 0);
                }else{
                    fprintf(fp, "%d", img->pixels[i][j].r);
                }
                if(img->type == PPM_ASCII || img->type == PBM_BINARY){
                    fprintf(fp, " ");
                    fprintf(fp, "%d", img->pixels[i][j].g);
                    fprintf(fp, " ");
                    fprintf(fp, "%d", img->pixels[i][j].b);
                }
                fprintf(fp, " ");
            }
            fprintf(fp, "\n");
        }
    }else{
        int i = 0, j = 0;
        for (int n = 0; n < img->width*img->height; ++n) {
            fprintf(fp, " ");

            if(img->type == PBM_BINARY || img->type == PBM_ASCII){
                //.pbm esetén 0 a fehér, és 1 a fekete
                fprintf(fp, "%d", img->pixels[i][j].r == 0 ? 1 : 0);
            }else{
                fprintf(fp, "%d", img->pixels[i][j].r);
            }
            if(img->type == PPM_ASCII || img->type == PBM_BINARY){
                fprintf(fp, " ");
                fprintf(fp, "%d", img->pixels[i][j].g);
                fprintf(fp, " ");
                fprintf(fp, "%d", img->pixels[i][j].b);
            }

            fprintf(fp, "\n");

            j++;
            if(img->width==j){
                j=0;
                i++;
            }
        }
    }
}

//Elmenti a képet egy paraméterként megadott névvel, aminek a fajl kiterjesztése, és a kép adata a paraméterként kapott Img változóban van benne
void mentes(Img* img, char* name) {
    char *newname = (char *)malloc(strlen(name)+5*sizeof(char));
    strcpy(newname, name);
    if (img->type == PBM_ASCII || img->type == PBM_BINARY){
        newname[strlen(name)]='.';
        newname[strlen(name)+1]='p';
        newname[strlen(name)+2]='b';
        newname[strlen(name)+3]='m';
        newname[strlen(name)+4]='\0';
    }else if(img->type == PGM_ASCII || img->type == PGM_BINARY){
        newname[strlen(name)]='.';
        newname[strlen(name)+1]='p';
        newname[strlen(name)+2]='g';
        newname[strlen(name)+3]='m';
        newname[strlen(name)+4]='\0';
    }else{
        newname[strlen(name)]='.';
        newname[strlen(name)+1]='p';
        newname[strlen(name)+2]='p';
        newname[strlen(name)+3]='m';
        newname[strlen(name)+4]='\0';
    }
    FILE* fp = fopen(newname, "wt");
    if(img->type == PBM_ASCII || img->type == PBM_BINARY){
        fprintf(fp, "P1");
        fprintf(fp, "\n");
        fprintf(fp, "%d %d", img->width, img->height);
        fprintf(fp, "\n");
    }else{
        fprintf(fp, "P2");
        fprintf(fp, "\n");
        fprintf(fp, "%d %d", img->width, img->height);
        fprintf(fp, "\n");
        fprintf(fp, "%d", img->maxValue);
        fprintf(fp, "\n");
    }

    printImgData(img, fp);
    fclose(fp);
    free(newname);
}
//Megnézi, hogy egy sor komment e
int testComment(char *buff){
    for (int i = 0; i < strlen(buff); ++i) {
        if (buff[i] == '#') {
            return 1;
        }
    }
    return 0;
}

//Egy PBM file tulajdonságait tölti be.
void readHEADERPBM(FILE* fp, Img* img) {
    char* buff = (char*)malloc(MaxLineSize * sizeof(char));

    fgets(buff, MaxLineSize, fp);
    while (fgets(buff, MaxLineSize, fp) != NULL) {
        //Leteszteli, hogy a sor komment e
        int comment = testComment(buff);

        if (comment == 0) {
            sscanf(buff, "%d %d", &img->width, &img->height);
            break;
        }
    }

    free(buff);
    img->maxValue = 1;
}
//Egy PPM/PGM file tulajdonságait tölti be.
void readHEADERPPMPGM(FILE* fp, Img* img) {
    char* buff = (char*)malloc(MaxLineSize * sizeof(char));
    fgets(buff, MaxLineSize, fp);
    int okIn = 0;
    while (fgets(buff, MaxLineSize, fp) != NULL) {
        //Leteszteli, hogy a sor komment e
        int comment = testComment(buff);

        if (comment == 0) {
            if (okIn == 0) {
                sscanf(buff, "%d %d", &img->width, &img->height);
                okIn = 1;
            }
            else {
                sscanf(buff, "%d", &img->maxValue);
                break;
            }
        }
    }

    free(buff);
}

//Egy ascii formátumú PBM/PGM file pixeleinek adatait tölti be.
void readPBMPGM(FILE *fp, Img* img) {
    
    if (img->type == PBM_ASCII) {
        readHEADERPBM(fp, img);
    }
    else {
        readHEADERPPMPGM(fp, img);
    }

    allocImg(img);

    int w = 0, h = 0;
    while(true){
        int color;
        if(fscanf(fp, "%d", &color) !=1) {
            break;
        }
        if(img->type==PBM_ASCII){
            //.pbm esetén 0 a fehér, és 1 a fekete
            //Ellentétes a .pgm- és a .ppm- el
            color = color == 0 ? 1 : 0;
        }
        img->pixels[h][w].r = color;
        img->pixels[h][w].g = color;
        img->pixels[h][w].b = color;
        w++;
        if(w==img->width){
            w = 0;
            h++;
            if(h == img->height) break;
        }
    }
}
//Egy ascii formátumú PPM file pixeleinek adatait tölti be.
void readPPM(FILE* fp, Img* img) {
    readHEADERPPMPGM(fp, img);

    allocImg(img);

    int w = 0, h = 0;
    while(true){
        int r, g, b;
        if(fscanf(fp, "%d %d %d", &r, &g, &b) != 3){
            break;
        }
        img->pixels[h][w].r = r;
        img->pixels[h][w].g = g;
        img->pixels[h][w].b = b;
        w++;
        if(w==img->width){
            w = 0;
            h++;
            if(h == img->height) break;
        }
    }
}

//Egy bináris formátumú PBM file pixeleinek adatait tölti be.
void readPBMBinary(FILE* fp, Img* img) {
    readHEADERPBM(fp, img);
    allocImg(img);

    char b;
    int i = 0;
    int x = 0;
    while (fread(&b, 1, 1, fp) == 1)
    {
        for (int a = 0; a < 8; a++) {
            img->pixels[i][x].r = (b >> (7 - a)) & 0x1;
            img->pixels[i][x].g = (b >> (7 - a)) & 0x1;
            img->pixels[i][x].b = (b >> (7 - a)) & 0x1;
            x++;
            if (x == img->width) {
                x = 0;
                a = 8;
                if (i == img->height-1) {
                    break;
                }
                else {
                    i++;
                }
            }
            
        }
    }
}
//Egy bináris formátumú PGM file pixeleinek adatait tölti be.
void readPGMBinary(FILE* fp, Img* img) {
    readHEADERPPMPGM(fp, img);
    allocImg(img);

    unsigned char* buff2 = (unsigned char*)malloc(img->width * img->height * sizeof(unsigned char));

    fread(buff2, img->width, img->height, fp);

    int n = 0;
    for (int i = 0; i < img->height; i++)
    {
        for (int x = 0; x < img->width; x++)
        {
            img->pixels[i][x].r = buff2[n];
            img->pixels[i][x].g = buff2[n];
            img->pixels[i][x].b = buff2[n];
            n ++;
        }
    }

    free(buff2);
}
//Egy bináris formátumú PPM file pixeleinek adatait tölti be.
void readPPMBinary(FILE* fp, Img* img) {
    readHEADERPPMPGM(fp, img);
    allocImg(img);

    unsigned char* buff2 = (unsigned char*)malloc(img->width * img->height * 3 * sizeof(unsigned char));
    fread(buff2, 3 * img->width, img->height, fp);

    int n = 0;
    for (int i = 0; i < img->height; i++)
    {
        for (int x = 0; x < img->width; x++)
        {
            img->pixels[i][x].r = buff2[n];
            img->pixels[i][x].g = buff2[n + 1];
            img->pixels[i][x].b = buff2[n + 2];
            n += 3;
        }
    }

    free(buff2);
}

//beolvas egy képet egy paraméterként megadott helyrõl, majd elmenti a paraméterként megadott pointerbe
int readImg(char* img_location, Img* img) {
    FILE* fp;
    fp = fopen(img_location, "rt");


    if (fp != NULL) {

        //Kép tulajdonságainak betöltése
        char* buff = (char*)malloc(3 * sizeof(char));
        fgets(buff, 3, fp);


        if (strcmp(buff, "P3") == 0) {
            img->type = PPM_ASCII;
            readPPM(fp, img);
        }
        else if (strcmp(buff, "P6") == 0) {
            img->type = PPM_BINARY;
            fclose(fp);
            fp = fopen(img_location, "rb");
            readPPMBinary(fp, img);
        }
        else if (strcmp(buff, "P1") == 0) {
            img->type = PBM_ASCII;
            readPBMPGM(fp, img);
        }
        else if (strcmp(buff, "P4") == 0) {
            img->type = PBM_BINARY;
            fclose(fp);
            fp = fopen(img_location, "rb");
            readPBMBinary(fp, img);
        }
        else if (strcmp(buff, "P2") == 0) {
            img->type = PGM_ASCII;
            readPBMPGM(fp, img);
        }
        else if (strcmp(buff, "P5") == 0) {
            img->type = PGM_BINARY;
            fclose(fp);
            fp = fopen(img_location, "rb");
            readPGMBinary(fp, img);
        }
        else {
            printf("Rossz formatumu kep! \n");
            img = NULL;
            fclose(fp);
            free(buff);
            return 0;
        }

        free(buff);

        fclose(fp);
        return 1;
    }
    else {
        printf("Nem lehet megnyitni a kepet! \n");
        img = NULL;
        return 0;
    }
}