//
// Created by so on 02-06-2024.
//

#include "requisito_2_2ºfase.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "monteCarlo.h"

// progress bar function
void *progressbar(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int num_points = data->num_points;
    int *checked_points = data->checked_points;
    pthread_mutex_t *mutex = data->mutex;

    while (1) {
        pthread_mutex_lock(mutex);
        int local_checked_points = *checked_points;
        pthread_mutex_unlock(mutex);

        int percentage = (local_checked_points * 100) / num_points;
        printf("\rprogress: %d%%", percentage);
        fflush(stdout);

        if (local_checked_points >= num_points) {
            break;
        }
        usleep(50000);
    }
    printf("\n");
    return NULL;
}

// working thread
void *Worker_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int points_per_thread = data->num_points / data->num_threads;
    int start = data->thread_id * points_per_thread;
    int end = start + points_per_thread;

    int local_count = 0;
    for (int i = start; i < end; i++) {
        if (isInsidePolygon(data->polygon, data->polygon_size, data->points[i])) {
            local_count++;
        } else {
            //printf("point (%lf, %lf) isn't inside the polygon.\n", data->points[i].x, data->points[i].y);
        }
    }

    pthread_mutex_lock(data->mutex);
    *(data->inside_count) += local_count;
    *(data->checked_points) += (end - start);
    pthread_mutex_unlock(data->mutex);

    return NULL;
}

// main function
void requisito_2_2ºfase(char *filename, int num_threads, int num_points) {
    srand(time(NULL));

    pthread_t threads[num_threads];
    pthread_t progress_thread;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int inside_count = 0;
    int checked_points = 0;
    Point *points = (Point *)malloc(num_points * sizeof(Point));

    int polygon_size;
    Point *polygon;
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("error opening file %s\n", filename);
        return;
    }
    fscanf(file, "%d", &polygon_size);
    printf("polygon size: %d\n", polygon_size);
    polygon = (Point *)malloc(polygon_size * sizeof(Point));
    for (int i = 0; i < polygon_size + 1; i++) {
        fscanf(file, "%lf %lf", &(polygon[i].x), &(polygon[i].y));
        //printf("point %d: (%lf, %lf)\n", i, polygon[i].x, polygon[i].y);
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
        data[i].checked_points = &checked_points;
        data[i].mutex = &mutex;
        data[i].polygon_size = polygon_size;
        data[i].polygon = polygon;
        pthread_create(&threads[i], NULL, Worker_thread, &data[i]);
    }

    // thread for progress bar
    pthread_create(&progress_thread, NULL, progressbar, &data[0]);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // wait termination of progress thread
    pthread_join(progress_thread, NULL);

    double area = 4 * inside_count / (double)num_points;
    printf("Estimated area: %f\n", area);
    printf("number of points inside the polygon: %d\n", inside_count);

    free(points);
    free(data);
    pthread_mutex_destroy(&mutex);
}