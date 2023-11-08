#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Chaos.h"

#define PI 3.1415926

void chaos2DLSM(_2dLSM *lsm, int n) {
    lsm->x = cos( 4 * 50 * lsm->x * (1 - lsm->x) + 50 * sin( PI * lsm->y ) + 1 );
    lsm->y = cos( 4 * 50 * lsm->y * (1 - lsm->y) + 50 * sin( PI * lsm->x ) + 1 );
}

void preiterChaos2DLSM(_2dLSM *lsm, int n) {
    chaos2DLSM(lsm, n);
}

void LSCMap(double *x_lscmap, int n) {
    int i = 0;
    double r = 0.4;
    for (i = 0; i < n; ++i) {
        *x_lscmap = cos(PI * (4*r* (*x_lscmap)* (1-*x_lscmap) + (1-r)*sin(PI * (*x_lscmap)) - 0.5));
    }
}

void preiterLSCMap(double *x_lscmap, int n) {
    n = 500;
    LSCMap(x_lscmap, n);
}
