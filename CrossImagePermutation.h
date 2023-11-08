#ifndef CROSSIMAGEPERMUTATION_H_INCLUDED
#define CROSSIMAGEPERMUTATION_H_INCLUDED

#include <math.h>
#include "Chaos.h"
#include "ReadBatchImages.h"

typedef struct {
    int* randImg;
    int* randPos;
} _randCoor;

void crossImgPermutation(batchImgInfo *imgsInfo, _randCoor *randCoor, _2dLSM *lsm, int round, int flag);
//void deCrossImgPermutation(batchImgInfo *imgsInfo, _randCoor *deRandCoor, _2dLSM *delsm, int round);

void freeRandCoor(_randCoor *randCoor);

#endif // CROSSIMAGEPERMUTATION_H_INCLUDED
