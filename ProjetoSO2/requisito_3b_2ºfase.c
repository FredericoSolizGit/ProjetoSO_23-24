//
// Created by so on 05-06-2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "monteCarlo.h"
#include "requisito_3b_2ºfase.h"

bool pointExists(Point *points, int num_points, Point p) {
    for (int i = 0; i < num_points; i++) {
        if (points[i].x == p.x && points[i].y == p.y) {
            return true;
        }
    }
    return false;
}

// producer thread
void* Producer_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->thread_id; i < data->num_points; i += data->num_threads) {
        Point p = {((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 2 - 1};

        pthread_mutex_lock(data->mutex);

        if(pointExists(data->points, i, p)){
        data->points[i] = p;
        }

        pthread_mutex_unlock(data->mutex);
    }

    pthread_exit(NULL);
}

// consumer thread
void* Consumer_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    while (1) {
        pthread_mutex_lock(data->mutex);
        while (*data->checked_points >= data->num_points) {
            pthread_mutex_unlock(data->mutex);
            pthread_exit(NULL);
        }

        Point p = data->points[*data->checked_points];
        (*data->checked_points)++;
        pthread_mutex_unlock(data->mutex);

        if (isInsidePolygon(data->polygon, data->polygon_size, p)) {
            pthread_mutex_lock(data->mutex);
            (*data->inside_count)++;
            pthread_mutex_unlock(data->mutex);
        }
    }
}

void requisito_3b_2ºfase(char *filename, int num_threads, int num_points) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    int polygon_size;
    if (fscanf(file, "%d", &polygon_size) != 1) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    Point *polygon = malloc(polygon_size * sizeof(Point));
    if (polygon == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < polygon_size; i++) {
        if (fscanf(file, "%lf %lf", &polygon[i].x, &polygon[i].y) != 2) {
            perror("error");
            free(polygon);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);

    pthread_t producers[num_threads], consumers[num_threads];
    ThreadData thread_data[num_threads];
    Point *points = malloc(num_points * sizeof(Point));
    if (points == NULL) {
        perror("error allocating memory points");
        free(polygon);
        exit(EXIT_FAILURE);
    }
    int inside_count = 0;
    int checked_points = 0;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    srand(time(NULL)); // seed

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].num_points = num_points;
        thread_data[i].points = points;
        thread_data[i].inside_count = &inside_count;
        thread_data[i].mutex = &mutex;
        thread_data[i].polygon_size = polygon_size;
        thread_data[i].polygon = polygon;
        thread_data[i].checked_points = &checked_points;

        pthread_create(&producers[i], NULL, Producer_thread, &thread_data[i]);
        pthread_create(&consumers[i], NULL, Consumer_thread, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
}


    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);

    printf("number of unique points inside the polygon: %d\n", inside_count);

    free(polygon);
    free(points);
    pthread_mutex_destroy(&mutex);
}