//
// Created by so on 02-06-2024.
//

#ifndef PROJETOSO2_MONTECARLO_H
#define PROJETOSO2_MONTECARLO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int thread_id;
    int num_threads;
    int num_points;
    Point *points;
    int *inside_count;
    pthread_mutex_t *mutex;
    int polygon_size;
    Point *polygon;
    int *checked_points;
} ThreadData;


Point generateRandomPoint();
int orientation(Point p, Point q, Point r);
bool onSegment(Point p, Point q, Point r);
bool doIntersect(Point p1, Point q1, Point p2, Point q2);
bool isInsidePolygon(Point polygon[], int n, Point p);
#endif //PROJETOSO2_MONTECARLO_H
