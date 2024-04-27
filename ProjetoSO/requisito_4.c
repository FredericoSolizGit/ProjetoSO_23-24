//
// Created by so on 23-04-2024.
//
#include "requisito_4.h"
#include "requisito_3.h"
#include "requisito_2.h"
#include "monteCarlo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

void handle_sigchld(int sig) {
    int status;
    wait(&status);
}

void requisito_4(char *filename, int num_processes, int total_points_generated, Point polygon[], int n) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (fd == -1) {
        printf("Erro ao abrir o ficheiro %s.\n", filename);
        return;
    }

    int pipes[num_processes][2]; // array de pipes para comunicação com cada processo filho

    for (int i = 0; i < num_processes; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return;
        }
    }

    int points_per_child = total_points_generated / num_processes;

    for (int i = 1; i < num_processes + 1; ++i) {
        signal(SIGCHLD, handle_sigchld);
        printf("Progresso: %d || Filho %d terminou.\n", (i* 100) / num_processes, i);

    }

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork falhou.\n");
            return;
        } else if (pid == 0) {

            srand(time(NULL) ^ getpid());

            int points_inside_polygon = 0;
            for (int j = 0; j < points_per_child; j++) {
                Point random_point = generateRandomPoint();
                char buffer[256];
                int len = snprintf(buffer, sizeof(buffer), "Process id: %d; Coordenada x: %.2f; Coordenada y: %.2f\n", getpid(), random_point.x, random_point.y);
                write(fd, buffer, len);
                if (isInsidePolygon(polygon, n, random_point)) {
                    points_inside_polygon++;
                }
            }

            char result_buffer[256];
            int result_len = snprintf(result_buffer, sizeof(result_buffer), "Process id:%d; Points generated by child: %d; Points inside polygon: %d;\n", getpid(), points_per_child, points_inside_polygon);
            write(fd, result_buffer, result_len);

            // Escrever a quantidade de pontos dentro do polígono para o pipe
            write(pipes[i][1], &points_inside_polygon, sizeof(int));
            close(pipes[i][1]); // Fechar lado de escrita do pipe no processo filho
            exit(0);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    int total_points_inside_polygon = 0;
    for (int i = 0; i < num_processes; i++) {
        int points_inside_polygon;
        read(pipes[i][0], &points_inside_polygon, sizeof(int));
        total_points_inside_polygon += points_inside_polygon;
        close(pipes[i][0]); // Fechar lado de leitura do pipe
    }

    double area = (double)total_points_inside_polygon / total_points_generated * 4.0;
    printf("Área do polígono: %f\n", area);
    close(fd);

    for (int i = 0; i < num_processes; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}