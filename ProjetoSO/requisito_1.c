//
// Created by so on 18-04-2024.
//

#include "requisito_1.h"
#include "monteCarlo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

void requisito_1(char *filename, int num_processes, int num_points_per_process) {
    int points_per_child = num_points_per_process / num_processes;

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        return;
    }

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork falhou.\n");
            return;
        } else if (pid == 0) {
            // Processo filho
            srand(time(NULL) ^ getpid());

            for (int j = 0; j < points_per_child; j++) {
                Point p = {((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 2 - 1};
                fprintf(file, "%.2f, %.2f\n", p.x, p.y);
            }

            fclose(file); // Fecha o arquivo antes de encerrar o processo filho
            exit(0);
        }
    }
    // Processo pai espera por todos os filhos terminarem
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    fclose(file);

    printf("Os pontos foram escritos em %s.\n", filename);
}
