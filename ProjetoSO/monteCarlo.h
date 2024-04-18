//
// Created by so on 18-04-2024.
//
#include <stdbool.h>
#ifndef PROJETOSO_MONTECARLO_H
#define PROJETOSO_MONTECARLO_H
typedef struct {
    double x;
    double y;
} Point;
Point generateRandomPoint();
int orientation(Point p, Point q, Point r);
bool onSegment(Point p, Point q, Point r);
bool doIntersect(Point p1, Point q1, Point p2, Point q2);
bool isInsidePolygon(Point polygon[], int n, Point p);

#endif //PROJETOSO_MONTECARLO_H
