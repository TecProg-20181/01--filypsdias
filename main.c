#include <stdio.h>

unsigned int counter;
unsigned int counter_two;

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    /* [width][height][rgb] */
    /* 0 -> r */
    /* 1 -> g */
    /* 2 -> b */
    Pixel pixel[512][512];
    unsigned int width;
    unsigned int height;
} Image;

/* Image functions */
Image scaleInGray();
Image sepia();
Image rotate();
Image mirroringImage();
Image cut();

/* Char function */
char readImageType();

/* Void functions */
void blur();
void invertColors();

/* Main function */
int main() {
    Image img;
    int max_color;
    char p3[4];

    readImageType(p3);
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    /* Read all pixels of image */
    for (counter = 0; counter < img.height; ++counter) {
        for (counter_two = 0; counter_two < img.width; ++counter_two) {
            scanf("%hu %hu %hu", &img.pixel[counter][counter_two].red,
                                 &img.pixel[counter][counter_two].green,
                                 &img.pixel[counter][counter_two].blue);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { /* Escala de Cinza */
                img = scaleInGray(img);
                break;
            }
            case 2: { /* Filtro Sepia */
                img = sepia(img);
                break;
            }
            case 3: { /* Filtro Borrar */
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height, img.pixel, tamanho, img.width);
                break;
            }
            case 4: { /* Rotacao */
                img = rotate(img);
                break;
            }
            case 5: { /* Filtro Espelhamento */
                img = mirroringImage(img);
                break;
            }
            case 6: { /* Inversao de Cores */
                invertColors(img.pixel, img.width, img.height);
                break;
            }
            case 7: { /* Cortar Imagem */
                img = cut(img);
                break;
            }
        }

    }

    /* Print type of image */
    printf("P3\n");

    /* Print width height and color of image */
    printf("%u %u\n255\n", img.width, img.height);

    /* Print pixels of image */
    for (counter = 0; counter < img.height; ++counter) {
        for (counter_two = 0; counter_two < img.width; ++counter_two) {
            printf("%hu %hu %hu ", img.pixel[counter][counter_two].red,
                                   img.pixel[counter][counter_two].green,
                                   img.pixel[counter][counter_two].blue);

        }
        printf("\n");
    }
    return 0;
}

/* Function to calculate the pixel's value average */
int calculatePixelAverage(Image img) {
    int average = img.pixel[counter][counter_two].red +
                  img.pixel[counter][counter_two].green +
                  img.pixel[counter][counter_two].blue;
    average /= 3;

    return average;
}

void transfeerPixelData(Image img, int average) {
    img.pixel[counter][counter_two].red = average;
    img.pixel[counter][counter_two].green = average;
    img.pixel[counter][counter_two].blue = average;
}

Image scaleInGray(Image img) {
    for (counter = 0; counter < img.height; ++counter) {
        for (unsigned int counter_two = 0; counter_two < img.width; ++counter_two) {
            int media = img.pixel[counter][counter_two].red +
                        img.pixel[counter][counter_two].green +
                        img.pixel[counter][counter_two].blue;
            media /= 3;
            img.pixel[counter][counter_two].red = media;
            img.pixel[counter][counter_two].green = media;
            img.pixel[counter][counter_two].blue = media;
        }
    }

    return img;
}

int max(int a, int b) {
    if (a > b)
        return b;
    return a;
}

Image sepia(Image img) {
    for (counter = 0; counter < img.height; ++counter) {
        for (counter_two = 0; counter_two < img.width; ++counter_two) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[counter][counter_two].red;
            pixel[1] = img.pixel[counter][counter_two].green;
            pixel[2] = img.pixel[counter][counter_two].blue;

            int p = pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = max(255, p);
            img.pixel[counter][counter_two].red = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = max(255, p);
            img.pixel[counter][counter_two].green = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255, p);
            img.pixel[counter][counter_two].blue = menor_r;
        }
    }
    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (counter = 0; counter < height; ++counter) {
        for (counter_two = 0; counter_two < width; ++counter_two) {
            Pixel media = {0, 0, 0};

            int menor_height = max(height - 1, counter + T/2);
            int min_width = max(width - 1, counter_two + T/2);
            for(int x = (0 > counter - T/2 ? 0 : counter - T/2); x <= menor_height; ++x) {
                for(int y = (0 > counter_two - T/2 ? 0 : counter_two - T/2); y <= min_width; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            pixel[counter][counter_two][0] = media.red;
            pixel[counter][counter_two][1] = media.green;
            pixel[counter][counter_two][2] = media.blue;
        }
    }
}


Image rotate90Right(Image img) {
    Image rotaded;

    rotaded.width = img.height;
    rotaded.height = img.width;

    for (int y = 0, counter = 0; counter < rotaded.height; ++counter, ++y) {
        for (int x = 0, counter_two = rotaded.width - 1; counter_two >= 0; --counter_two, ++x) {
            rotaded.pixel[counter][counter_two].red = img.pixel[x][y].red;
            rotaded.pixel[counter][counter_two].green = img.pixel[x][y].green;
            rotaded.pixel[counter][counter_two].blue = img.pixel[x][y].blue;
        }
    }

    return rotaded;
}

Image rotate(Image img) {
    int quantas_vezes = 0;
    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    for (int j = 0; j < quantas_vezes; ++j) {
        img = rotate90Right(img);
    }
    return img;
}

Image mirroringImage(Image img) {
    int horizontal = 0;
    int w, h;
    scanf("%d", &horizontal);
    w = img.width;
    h = img.height;
    
    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (counter = 0; counter < h; ++counter) {
        for (counter_two = 0; counter_two < w; ++counter_two) {
            int x = counter, y = counter_two;

            if (horizontal == 1) {
            y = img.width - 1 - counter_two;
            } else {
                x = img.height - 1 - counter;
            }
            Pixel aux1;
            aux1.red = img.pixel[counter][counter_two].red;
            aux1.green = img.pixel[counter][counter_two].green;
            aux1.blue = img.pixel[counter][counter_two].blue;

            img.pixel[counter][counter_two].red = img.pixel[x][y].red;
            img.pixel[counter][counter_two].green = img.pixel[x][y].green;
            img.pixel[counter][counter_two].blue = img.pixel[x][y].blue;

            img.pixel[x][y].red = aux1.red;
            img.pixel[x][y].green = aux1.green;
            img.pixel[x][y].blue = aux1.blue;
        }
    }
    return img;
}

void invertColors(unsigned short int pixel[512][512][3], unsigned int w, unsigned int h) {
    for (counter = 0; counter < h; ++counter) {
        for (counter_two = 0; counter_two < w; ++counter_two) {
            pixel[counter][counter_two][0] = 255 - pixel[counter][counter_two][0];
            pixel[counter][counter_two][1] = 255 - pixel[counter][counter_two][1];
            pixel[counter][counter_two][2] = 255 - pixel[counter][counter_two][2];
        }
    }
}

Image cutImage(Image img, int x, int y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int counter = 0; counter < height; ++counter) {
        for(int counter_two = 0; counter_two < width; ++counter_two) {
            cortada.pixel[counter][counter_two].red = img.pixel[counter + y][counter_two + x].red;
            cortada.pixel[counter][counter_two].green = img.pixel[counter + y][counter_two + x].green;
            cortada.pixel[counter][counter_two].blue = img.pixel[counter + y][counter_two + x].blue;
        }
    }

    return cortada;
}

Image cut(Image img){
    int x, y;
    int w, h;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &w, &h);
    img = cutImage(img, x, y, w, h);

    return img;
}

/* Function that reads the Image Type */
char readImageType(char *pixels) {
    scanf("%s", pixels);
    return *pixels;
}

/* Function to scan two int values */
void scanIntValues(int value1, int value2) {
    scanf("%d %d",&value1, &value2);
}