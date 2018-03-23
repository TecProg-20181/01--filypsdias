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
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

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
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

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
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue;
            media /= 3;
            img.pixel[i][j].red = media;
            img.pixel[i][j].green = media;
            img.pixel[i][j].blue = media;
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
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[i][j].red;
            pixel[1] = img.pixel[i][j].green;
            pixel[2] = img.pixel[i][j].blue;

            int p = pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = max(255, p);
            img.pixel[i][j].red = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = max(255, p);
            img.pixel[i][j].green = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255, p);
            img.pixel[i][j].blue = menor_r;
        }
    }
    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_height = max(height - 1, i + T/2);
            int min_width = max(width - 1, j + T/2);
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_height; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_width; ++y) {
                    media.red += pixel[x][y][0];
                    media.green += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            pixel[i][j][0] = media.red;
            pixel[i][j][1] = media.green;
            pixel[i][j][2] = media.blue;
        }
    }
}


Image rotate90Right(Image img) {
    Image rotaded;

    rotaded.width = img.height;
    rotaded.height = img.width;

    for (int y = 0, i = 0; i < rotaded.height; ++i, ++y) {
        for (int x = 0, j = rotaded.width - 1; j >= 0; --j, ++x) {
            rotaded.pixel[i][j].red = img.pixel[x][y].red;
            rotaded.pixel[i][j].green = img.pixel[x][y].green;
            rotaded.pixel[i][j].blue = img.pixel[x][y].blue;
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

    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            int x = i, y = j;

            if (horizontal == 1) {
            y = img.width - 1 - j;
            } else {
                x = img.height - 1 - i;
            }
            Pixel aux1;
            aux1.red = img.pixel[i][j].red;
            aux1.green = img.pixel[i][j].green;
            aux1.blue = img.pixel[i][j].blue;

            img.pixel[i][j].red = img.pixel[x][y].red;
            img.pixel[i][j].green = img.pixel[x][y].green;
            img.pixel[i][j].blue = img.pixel[x][y].blue;

            img.pixel[x][y].red = aux1.red;
            img.pixel[x][y].green = aux1.green;
            img.pixel[x][y].blue = aux1.blue;
        }
    }
    return img;
}

void invertColors(unsigned short int pixel[512][512][3], unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cutImage(Image img, int x, int y, int width, int height) {
    Image cortada;

    cortada.width = width;
    cortada.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j].red = img.pixel[i + y][j + x].red;
            cortada.pixel[i][j].green = img.pixel[i + y][j + x].green;
            cortada.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
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