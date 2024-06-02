//
// Created by so on 02-06-2024.
//

#include "requisito_2_2ºfase.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "monteCarlo.h"

void *Worker_thread(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    int points_per_thread = data->num_points / data->num_threads;
    int start = data->thread_id * points_per_thread;
    int end = start + points_per_thread;


    for (int i = start; i < end; i++) {
        if (isInsidePolygon(data->polygon, data->polygon_size, data->points[i])) {
            (*data->inside_count)++;
        }
    }

    pthread_mutex_lock(data->mutex);
    pthread_mutex_unlock(data->mutex);
    usleep(1000);

    return NULL;
}

void *progressbar(void *arg) {
    int *num_points = (int *) arg;
    int i = 0;
    while (i < *num_points) {
        printf("\rProgresso: %d%%", (i * 100) / *num_points);
        fflush(stdout);
        usleep(1000);
        i++;
    }
    printf("\n");
    return NULL;
}

void requisito_2_2ºfase(char *filename, int num_threads, int num_points) {

    srand(time(NULL));


    pthread_t threads[num_threads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int inside_count = 0;
    Point points[num_points];

    for (int i = 0; i < num_points; i++) {
        Point p = {((double) rand() / RAND_MAX) * 2 - 1, ((double) rand() / RAND_MAX) * 2 - 1};
        points[i] = p;
    }


    ThreadData *data = malloc(num_threads * sizeof(ThreadData));
    for (int i = 0; i < num_threads; i++) {
        data[i].thread_id = i;
        data[i].num_threads = num_threads;
        data[i].num_points = num_points;
        data[i].points = points;
        data[i].inside_count = &inside_count;
        data[i].mutex = &mutex;
        pthread_create(&threads[i], NULL, Worker_thread, &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    int area = 4 * inside_count/num_points;
    printf("Área estimada: %d\n", area);


    printf("Número de pontos dentro do polígono: %d\n", inside_count);
    free(points);
    free(data);
    pthread_mutex_destroy(&mutex);
}

