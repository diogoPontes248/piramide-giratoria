#include <stdio.h>
#include <string.h>
#include <math.h>

float A = 0.0, B = 0.0, C = 0.0;

float cubeWidth = 30;
int width = 121;
int height = 29;
float zBuffer[121* 29];
char buffer[121* 29];
int backgroundASCIICode = ' ';
int distanceFromCam = 150;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k){
    return j* sin(A) * sin(B) * cos(C) - k*cos(A)* sin(B) * cos(C) +
           j* cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k){
    return j* cos(A) * cos(C) + k * sin(A) * cos(C) -
           j* sin(A) * sin(B) *sin(C) + k * cos(A) * sin(B) * sin(C) -
           i* cos(B) * sin(C);
}

float calculateZ(int i, int j, int k){
    return k* cos(A) * cos(B) - j* sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch){
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1/z;

    xp = (int) (width /2 + K1 * ooz * x * 2);
    yp = (int) (height/2 + K1 * ooz * y);

    idx = xp + yp * width;
    if(idx >= 0 && idx < width * height){
        if(ooz > zBuffer[idx]){
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

float modulo(float x){
    if(x < 0){
        return (-x);
    }
    return x;
}

int main()
{
    printf("\x1b[2J");
    while(1){
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);

        for(float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed){
            for(float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed){
                for(float cubeZ = -cubeWidth; cubeZ < cubeWidth; cubeZ += incrementSpeed){
                    int teste1 = cubeY < -2 * cubeX + cubeWidth;
                    int teste2 = cubeY < 2 * cubeX + cubeWidth;
                    int teste3 = cubeY < -2 * cubeZ + cubeWidth;
                    int teste4 = cubeY < 2 * cubeZ + cubeWidth;

                    if(cubeY < -2 * cubeX + cubeWidth + 1 && cubeY > -2 * cubeX + cubeWidth - 1 && teste3 && teste4){
                        calculateForSurface(cubeX, cubeY, cubeZ, ';');
                    }
                    if(cubeY < 2 * cubeX + cubeWidth + 1 && cubeY > 2 * cubeX + cubeWidth - 1 && teste3 && teste4){
                        calculateForSurface(cubeX, cubeY, cubeZ, '~');
                    }
                    if(cubeY < -2 * cubeZ + cubeWidth + 1 && cubeY > -2 * cubeZ + cubeWidth - 1 && teste1 && teste2){
                        calculateForSurface(cubeX, cubeY, cubeZ, '$');
                    }
                    if(cubeY < 2 * cubeZ + cubeWidth + 1 && cubeY > 2 * cubeZ + cubeWidth - 1 && teste1 && teste2){
                        calculateForSurface(cubeX, cubeY, cubeZ, '#');
                    }
                    if(cubeY < -cubeWidth + 1){
                        calculateForSurface(cubeX, cubeY, cubeZ, '.');
                    }
                }
            }
        }

        printf("\x1b[H");

        for(int k = 0; k < width * height; k++){
            putchar(k% width ? buffer[k] : 10);
        }

        A += 0.09;
        B += 0.09;
        C += 0.09;
    }
    return 0;
}
