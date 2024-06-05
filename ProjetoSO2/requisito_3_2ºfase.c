//
// Created by so on 05-06-2024.
//

#include "requisito_3_2ºfase.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "monteCarlo.h"


// producer thread
void* producer_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    for (int i = data->thread_id; i < data->num_points; i += data->num_threads) {
        Point p = {((double)rand() / RAND_MAX) * 2 - 1, ((double)rand() / RAND_MAX) * 2 - 1};

        pthread_mutex_lock(data->mutex);
        data->points[i] = p;
        pthread_mutex_unlock(data->mutex);
    }

    pthread_exit(NULL);
}

// consumer thread
void* consumer_thread(void *arg) {
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
//main function
void requisito_3_2ºfase(char *filename, int num_threads, int num_points) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }

    int polygon_size;
    fscanf(file, "%d", &polygon_size);
    Point *polygon = malloc(polygon_size * sizeof(Point));
    for (int i = 0; i < polygon_size; i++) {
        fscanf(file, "%lf %lf", &polygon[i].x, &polygon[i].y);
    }
    fclose(file);

    pthread_t producers[num_threads], consumers[num_threads];
    ThreadData thread_data[num_threads];
    Point *points = malloc(num_points * sizeof(Point));
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

        pthread_create(&producers[i], NULL, producer_thread, &thread_data[i]);
        pthread_create(&consumers[i], NULL, consumer_thread, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    printf("number of points inside the polygon: %d\n", inside_count);

    double area = 4 * inside_count / (double)num_points;
    printf("Estimated area: %f\n", area);

    free(polygon);
    free(points);
    pthread_mutex_destroy(&mutex);
}
