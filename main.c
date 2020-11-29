#include <stdio.h>
#include "imageEditing.h"
#include "imageReading.h"
#include "debugmalloc.h"


int main() {
    printf("Add meg a kepfajl helyet: ");
    char img_location[2048];
    scanf("%s", img_location);

    Img* img = (Img*)malloc(sizeof(Img));

    int exit = 0;
    while (exit == 0){
        if (readImg(img_location, img) == 1) {
            int utasitas = 0;
            int runAgain = 0;
            do {
                printf("Add meg az utasitas szamat:  \n");
                printf("1: fenyerovaltoztatas, 2: kontrasztnoveles, 3: elmosas, 4: elfekete-feherites, 5: binaris keppe valo atalakitas, 6: mentes, 7: kilepes, 8: uj kep megnyitasa \n");

                scanf("%d", &utasitas);
                switch (utasitas) {
                    default: {
                        break;
                    }
                    case 1: {
                        printf("Add meg a fenyero nagysagat (+-%d): ", img->maxValue);
                        int brightness = 0;
                        scanf("%d", &brightness);
                        fenyerovaltoztatas(img, brightness);
                        break;
                    }
                    case 2: {
                        printf("Add meg a contrast nagysagat: (+- 300)");
                        int contrast = 0;
                        scanf("%d", &contrast);
                        contrastvaltoztatas(img, contrast);
                        break;
                    }
                    case 3: {
                        printf("Add meg az elmosas merteket: ");
                        int x = 0;
                        scanf("%d", &x);
                        blur(img, x);
                        break;
                    }
                    case 4: {
                        printf("Elfeketefeherites \n");
                        grayscale(img);
                        break;
                    }
                    case 5: {
                        printf("Binaris keppe valo atalakitas \n");
                        convertToBinaryImage(img);
                        break;
                    }
                    case 6: {
                        printf("Add meg az uj kep nevet: \n");
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
            printf("Add meg a kepfajl helyet: ");
            scanf("%s", img_location);
        }
    }

    free(img);
    return 0;
}
