#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include "monteCarlo.h"

void child_process(int id, int pipes[2], Point polygon[], int n, const char* filename) {
    int points_inside = 0;
    FILE *fp = fopen(filename, "a");  // Open file in append mode

    while (1) {
        // Receive a point from parent process
        Point p;
        int nread = read(pipes[0], &p, sizeof(p));
        if (nread == 0) {
            break;  // No more points to process
        }
        if (nread < 0) {
            perror("Error reading from pipe");
            exit(1);
        }

        // Check if point is inside the polygon
        if (isInsidePolygon(polygon, n, p)) {
            points_inside++;
        }

        // Write point coordinates to pipe
        int nwritten = write(pipes[1], &p, sizeof(p));
        if (nwritten < 0) {
            perror("Error writing to pipe");
            exit(1);
        }
    }

    // Write result to file (child's id and points inside)
    fprintf(fp, "Process %d: Points inside = %d\n", id, points_inside);
    fclose(fp);

    // Send result back to parent process
    int nwritten = write(pipes[1], &points_inside, sizeof(int));
    if (nwritten < 0) {
        perror("Error writing to pipe");
        exit(1);
    }
}


