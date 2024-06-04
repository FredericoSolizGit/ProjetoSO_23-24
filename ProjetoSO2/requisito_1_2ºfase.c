//
// Created by so on 02-06-2024.
//

#include "requisito_1_2ºfase.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "requisito_1_2ºfase.h"
#include "monteCarlo.h"



// Função da thread de trabalho
void *worker_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int points_per_thread = data->num_points / data->num_threads;
    int start = data->thread_id * points_per_thread;
    int end = start + points_per_thread;

    int local_count = 0;
    for (int i = start; i < end; i++) {
        if (isInsidePolygon(data->polygon, data->polygon_size, data->points[i])) {
            local_count++;
        } else {
            //printf("Ponto (%lf, %lf) não está dentro do polígono.\n", data->points[i].x, data->points[i].y);
        }
    }

    pthread_mutex_lock(data->mutex);
    *(data->inside_count) += local_count;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

// Função principal (requisito_1_2fase)
void requisito_1_2ºfase(char *filename, int num_threads, int num_points) {
    srand(time(NULL));

    pthread_t threads[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int inside_count = 0;
    Point *points = (Point *)malloc(num_points * sizeof(Point));

    // Lê o polígono do arquivo
    int polygon_size;
    Point *polygon;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    fscanf(file, "%d", &polygon_size);
    printf("Tamanho do polígono: %d\n", polygon_size);
    polygon = (Point *)malloc(polygon_size * sizeof(Point));
    for (int i = 0; i < polygon_size + 1; i++) {
        fscanf(file, "%lf %lf", &(polygon[i].x), &(polygon[i].y));
        //printf("Ponto %d: (%lf, %lf)\n", i, polygon[i].x, polygon[i].y);
    }
    fclose(file);

    for (int i = 0; i < num_points; i++) {
        Point p = {((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 2 - 1};
        points[i] = p;
    }

    ThreadData *data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));
    for (int i = 0; i < num_threads; i++) {
        data[i].thread_id = i;
        data[i].num_threads = num_threads;
        data[i].num_points = num_points;
        data[i].points = points;
        data[i].inside_count = &inside_count;
        data[i].mutex = &mutex;
        data[i].polygon_size = polygon_size;
        data[i].polygon = polygon;
        pthread_create(&threads[i], NULL, worker_thread, &data[i]);
    }


    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double area = 4 * inside_count / (double)num_points;
    printf("Área estimada: %f\n", area);
    printf("Número de pontos dentro do polígono: %d\n", inside_count);

    free(points);
    free(data);
    pthread_mutex_destroy(&mutex);
}


/*
void *worker_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int points_per_thread = data->num_points / data->num_threads;
    int start = data->thread_id * points_per_thread;
    int end = start + points_per_thread;
/*
    printf("Thread %d: Polígono:\n", data->thread_id);
    for (int i = 0; i < (data->polygon_size); i++) {
        printf("(%lf, %lf) ", data->polygon[i].x, data->polygon[i].y);
    }
    printf("\n");

    printf("Thread %d: Pontos gerados:\n", data->thread_id);
    for (int i = start; i < end; i++) {
        printf("(%lf, %lf) ", data->points[i].x, data->points[i].y);
        if (data->points[i].x < -1 || data->points[i].x > 1 || data->points[i].y < -1 || data->points[i].y > 1) {
            printf("[Fora do intervalo esperado!]");
        }
        printf("\n");
    }
    */
/*

    int local_count = 0;
    for (int i = start; i < end; i++) {
        if (isInsidePolygon(data->polygon, (data->polygon_size), data->points[i])) {
            local_count++;
        } else {
            //printf("Ponto (%lf, %lf) não está dentro do polígono.\n", data->points[i].x, data->points[i].y);
        }
    }

    pthread_mutex_lock(data->mutex);
    *(data->inside_count) += local_count;
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

void requisito_1_2ºfase(char *filename, int num_threads, int num_points) {
    srand(time(NULL));

    pthread_t threads[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int inside_count = 0;
    Point *points = (Point *)malloc(num_points * sizeof(Point));

    // Lê o polígono do arquivo
    int polygon_size;
    Point *polygon;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    fscanf(file, "%d", &polygon_size);
    printf("Tamanho do polígono: %d\n", polygon_size);
    polygon = (Point *)malloc(polygon_size * sizeof(Point));
    for (int i = 0; i < polygon_size + 1; i++) {
        fscanf(file, "%lf %lf", &(polygon[i].x), &(polygon[i].y));
        //printf("Ponto %d: (%lf, %lf)\n", i, polygon[i].x, polygon[i].y);
    }
    fclose(file);

    for (int i = 0; i < num_points; i++) {
        Point p = {((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 2 - 1};
        points[i] = p;
    }

    ThreadData *data = (ThreadData *)malloc(num_threads * sizeof(ThreadData));
    for (int i = 0; i < num_threads; i++) {
        data[i].thread_id = i;
        data[i].num_threads = num_threads;
        data[i].num_points = num_points;
        data[i].points = points;
        data[i].inside_count = &inside_count;
        data[i].mutex = &mutex;
        data[i].polygon_size = polygon_size;
        data[i].polygon = polygon;
        pthread_create(&threads[i], NULL, worker_thread, &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double area = 4 * inside_count / num_points;
    printf("Área estimada: %f\n", area);

    printf("Número de pontos dentro do polígono: %d\n", inside_count);
    free(points);
    free(data);
    pthread_mutex_destroy(&mutex);
}
*/