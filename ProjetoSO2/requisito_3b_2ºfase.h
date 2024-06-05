//
// Created by so on 05-06-2024.
//

#ifndef PROJETOSO2_REQUISITO_3B_2ºFASE_H
#define PROJETOSO2_REQUISITO_3B_2ºFASE_H
bool pointExists(Point *points, int num_points, Point p);
void* Producer_thread(void *arg);
void* Consumer_thread(void *arg);
void requisito_3b_2ºfase(char *filename, int num_threads, int num_points);
#endif //PROJETOSO2_REQUISITO_3B_2ºFASE_H
