#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "monteCarlo.h"

#define SOCKET_PATH "/tmp/monteCarlo.socket"

void removeSocketFile() {
    if (remove(SOCKET_PATH) == -1) {
        perror("Erro ao remover o arquivo de soquete");
        exit(EXIT_FAILURE);
    }
}

void requisito_5(char *filename, int num_processes, int total_points_generated, Point polygon[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", filename);
        return;
    }

    // Remover o arquivo de soquete se existir
    removeSocketFile();

    int sockfd, newsockfd;
    struct sockaddr_un serv_addr, cli_addr;
    socklen_t clilen;

    // Permitir reutilização do endereço do socket
    int opt = 1;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar o socket");
        return;
    }

    // Definir a opção SO_REUSEADDR
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Erro ao definir a opção SO_REUSEADDR");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erro ao fazer o bind");
        return;
    }

    listen(sockfd, num_processes);
    clilen = sizeof(cli_addr);

    int points_per_child = total_points_generated / num_processes;

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork falhou.\n");
            return;
        } else if (pid == 0) {
            close(sockfd); // Fechar o socket no processo filho
            srand(time(NULL) ^ getpid());

            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (newsockfd < 0) {
                perror("Erro ao aceitar conexão");
                exit(EXIT_FAILURE);
            }


            int points_inside_polygon = 0;
            for (int j = 0; j < points_per_child; j++) {
                Point random_point = generateRandomPoint();
                fprintf(file, "Process id: %d; Coordenada x: %.2f; Coordenada y: %.2f\n", getpid(), random_point.x, random_point.y);
                if (isInsidePolygon(polygon, n, random_point)) {
                    points_inside_polygon++;
                }
            }

            fprintf(file, "Process id:%d; Points generated by child: %d; Points inside polygon: %d;\n", getpid(), points_per_child, points_inside_polygon);

            write(newsockfd, &points_inside_polygon, sizeof(int));
            close(newsockfd);
            fclose(file); // Fechar o arquivo no processo filho
            exit(EXIT_SUCCESS);
        }
    }

    // Aguardar todos os processos filhos
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    int total_points_inside_polygon = 0;
    for (int i = 0; i < num_processes; i++) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Erro ao aceitar conexão");
            exit(EXIT_FAILURE);
        }

        int points_inside_polygon;
        read(newsockfd, &points_inside_polygon, sizeof(int));
        total_points_inside_polygon += points_inside_polygon;

        close(newsockfd);
    }

    double area = (double) total_points_inside_polygon / total_points_generated * 4.0;
    printf("Área do polígono: %f\n", area);

    // Fechar o socket e remover o arquivo de soquete
    close(sockfd);
    removeSocketFile();
}