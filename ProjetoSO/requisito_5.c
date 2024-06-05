//
// Created by so on 27-04-2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "monteCarlo.h"

#define SOCKET_PATH "/tmp/polygon_socket"
#define MAX_BACKLOG 5

int socket_already_in_use(const char *socket_path) {
    return access(socket_path, F_OK) != -1;
}

void remove_existing_socket(const char *socket_path) {
    if (unlink(socket_path) == -1) {
        perror("error removing socket");
        exit(EXIT_FAILURE);
    }
}

void requisito_5(char *filename, int num_processes, int total_points_generated) {

    if (socket_already_in_use(SOCKET_PATH)) {
        remove_existing_socket(SOCKET_PATH);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("error opening file %s.\n", filename);
        return;
    }

    int n;
    fscanf(file, "%d", &n);

    Point *polygon = (Point *)malloc(n * sizeof(Point));
    if (polygon == NULL) {
        printf("error allocating.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf %lf", &polygon[i].x, &polygon[i].y);
    }

    fclose(file);

    // create a socket
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("error creating socket");
        free(polygon);
        return;
    }

    struct sockaddr_un server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("error binding socket");
        free(polygon);
        close(server_fd);
        return;
    }

    if (listen(server_fd, MAX_BACKLOG) == -1) {
        perror("error listening on socket");
        free(polygon);
        close(server_fd);
        return;
    }

    // accept connection from client
    for (int i = 0; i < num_processes; i++) {
        struct sockaddr_un client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            perror("error connecting with client");
            free(polygon);
            close(server_fd);
            return;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            free(polygon);
            close(server_fd);
            close(client_fd);
            return;
        } else if (pid == 0) {
            // child
            int points_inside_polygon = 0;
            Point random_point;
            for (int j = 0; j < total_points_generated / num_processes; j++) {
                read(client_fd, &random_point, sizeof(random_point));
                if (isInsidePolygon(polygon, n, random_point)) {
                    points_inside_polygon++;
                }
            }
            close(client_fd);
            free(polygon);
            exit(points_inside_polygon);
        } else {
            close(client_fd); // closing client connection socket in main process
        }
    }

    // wait termination of child processes
    int total_points_inside_polygon = 0;
    for (int i = 0; i < num_processes; i++) {
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            total_points_inside_polygon += WEXITSTATUS(status);
        }
    }

    double area = (double)total_points_inside_polygon / total_points_generated * 4.0;
    printf("polygon area: %f\n", area);

    close(server_fd);
    unlink(SOCKET_PATH);
    free(polygon);
}

/*
#define SOCKET_PATH "/tmp/socket"

void requisito_5(char *filename, int num_processes, int total_points_generated, Point polygon[], int n) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (fd == -1) {
        printf("Erro ao abrir o ficheiro %s.\n", filename);
        return;
    }

    int server_fd, new_socket;
    struct sockaddr_un address;

    // cria socket
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("erro socket");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Removendo qualquer socket anterior com o mesmo nome
    unlink(SOCKET_PATH);

    // associa socket ao caminho
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) == -1) {
        perror("bind falhou");
        exit(EXIT_FAILURE);
    }

    // socket em modo escuta
    if (listen(server_fd, num_processes) < 0) {
        perror("listen falhou");
        exit(EXIT_FAILURE);
    }

    int points_per_child = total_points_generated / num_processes;

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork falhou.\n");
            return;
        } else if (pid == 0) {
            // fecha socket de escuta no processo filho
            close(server_fd);

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

            // Comunicação com o processo pai através do socket
            if ((new_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                perror("socket falhou");
                exit(EXIT_FAILURE);
            }

            if (connect(new_socket, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) == -1) {
                perror("connect falhou");
                exit(EXIT_FAILURE);
            }

            // Enviar o número de pontos dentro do polígono para o processo pai
            send(new_socket, &points_inside_polygon, sizeof(int), 0);
            close(new_socket);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    int total_points_inside_polygon = 0;
    for (int i = 0; i < num_processes; i++) {
        int points_inside_polygon;
        if ((new_socket = accept(server_fd, NULL, NULL)) == -1) {
            perror("accept falhou");
            exit(EXIT_FAILURE);
        }
        recv(new_socket, &points_inside_polygon, sizeof(int), 0);
        total_points_inside_polygon += points_inside_polygon;
        close(new_socket);
    }

    double area = (double)total_points_inside_polygon / total_points_generated * 4.0;
    printf("Área do polígono: %f\n", area);
    close(fd);

    close(server_fd);

    // remove socket
    unlink(SOCKET_PATH);
}
 */