//
// Created by so on 18-04-2024.
//

#include "requisito_1.h"
#include "monteCarlo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>
;

void requisito_1(char *filename, int num_processes, int num_points_per_process) {
    int points_per_child = num_points_per_process / num_processes;

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (fd == -1) {
        printf("Erro ao abrir o ficheiro %s.\n", filename);
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
                char buffer[64];
                int len = snprintf(buffer, sizeof(buffer), "%.2f, %.2f\n", p.x, p.y);
                if (write(fd, buffer, len) == -1) {
                    perror("Erro ao escrever no ficheiro");
                    exit(EXIT_FAILURE);
                }
            }

            close(fd);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    close(fd);

    printf("Os pontos foram escritos em %s.\n", filename);
}
