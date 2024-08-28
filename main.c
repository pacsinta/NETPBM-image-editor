#include <stdio.h>
#include "imageEditing.h"
#include "imageReading.h"
#include "debugmalloc.h"


int main() {
    printf("Open a NETBPM image: ");
    char img_location[2048];
    scanf("%s", img_location);

    Img* img = (Img*)malloc(sizeof(Img));

    int exit = 0;
    while (exit == 0){
        if (readImg(img_location, img) == 1) {
            int utasitas = 0;
            int runAgain = 0;
            do {
                printf("Choose form the following options:  \n");
                printf("1: edit brightness, 2: change contrast, 3: add blur, 4: grayscale, 5: make the image binary black&white, 6: save, 7: exit, 8: open a new image \n");

                scanf("%d", &utasitas);
                switch (utasitas) {
                    default: {
                        break;
                    }
                    case 1: {
                        printf("Set the brightness value (+-%d): ", img->maxValue);
                        int brightness = 0;
                        scanf("%d", &brightness);
                        fenyerovaltoztatas(img, brightness);
                        break;
                    }
                    case 2: {
                        printf("Set the contrast value: (+- 300)");
                        int contrast = 0;
                        scanf("%d", &contrast);
                        contrastvaltoztatas(img, contrast);
                        break;
                    }
                    case 3: {
                        printf("Set the blur value: ");
                        int x = 0;
                        scanf("%d", &x);
                        blur(img, x);
                        break;
                    }
                    case 4: {
                        printf("Making the grayscale image \n");
                        grayscale(img);
                        break;
                    }
                    case 5: {
                        printf("Making the black&white image \n");
                        convertToBinaryImage(img);
                        break;
                    }
                    case 6: {
                        printf("Set the new image name: \n");
                        char name[2048];
                        scanf("%s", name);
                        mentes(img, name);
                        break;
                    }
                    case 7: {
                        exit = 1;
                        runAgain = 1;
                        freeImg(img);
                        break;
                    }
                    case 8: {
                        freeImg(img);
                        runAgain = 1;
                        break;
                    }
                }
            } while (runAgain == 0);
        }

        if(exit == 0){
            printf("Set the image location: ");
            scanf("%s", img_location);
        }
    }

    free(img);
    return 0;
}
