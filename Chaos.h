#ifndef CHAOS_H_INCLUDED
#define CHAOS_H_INCLUDED

typedef struct {
    float x;
    float y;
} _2dLSM;

void preiterChaos2DLSM(_2dLSM *lsm, int n);
void chaos2DLSM(_2dLSM *lsm, int n);

void preiterLSCMap(double *x_lscmap, int n);
void LSCMap(double *x_lscmap, int n);


#endif // CHAOS_H_INCLUDED
