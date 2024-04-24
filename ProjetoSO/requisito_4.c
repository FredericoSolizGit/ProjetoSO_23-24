//
// Created by so on 23-04-2024.
//

#include "requisito_4.h"
#include <unistd.h>

void printProgressBar(int totalPoints, int verifiedPoints) {

    float progress = (float)verifiedPoints / totalPoints * 100;

    printf("\rProgresso: [");
    int pos;
    for (pos = 0; pos < 50; ++pos) {
        if (pos < progress / 2)
            printf("=");
        else if (pos == progress / 2)
            printf(">");
        else
            printf(" ");
    }
    printf("] %.2f%%", progress);
    fflush(stdout);
}