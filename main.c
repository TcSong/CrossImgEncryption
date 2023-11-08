#include <stdio.h>
#include <stdlib.h>
#include "Chaos.h"
#include "ReadBatchImages.h"
#include "CrossImagePermutation.h"
#include "CrossImageDiffusion.h"


int main(int argc, char* argv[])
{
    //int i;
    _2dLSM *lsm = (_2dLSM*)malloc(sizeof(_2dLSM));
    _2dLSM *delsm = (_2dLSM*)malloc(sizeof(_2dLSM));
    lsm->x = 0.234878;//8
    lsm->y = 0.378945;
    delsm->x = 0.234879;
    delsm->y = 0.378945;

    double lscVal = 0.54437346332574;//4
    double lscVal1 = 0.32437343432556;
    double delscVal = 0.54437346332574;
    double delscVal1 = 0.32437343432556;


    imgNum = readBatchImages(argv[1]);          //获取argv[1]中图像的数目
    batchImgInfo *imgsInfo = (batchImgInfo*)malloc(sizeof(batchImgInfo) * imgNum);          //开辟内存空间
    loadBatchImageInfo(imgsInfo);               //获取每张图像的信息，width,height,len等

    _randCoor *randCoor = (_randCoor*)malloc(sizeof(_randCoor) * imgNum);
    //crossImgPermutation(imgsInfo, randCoor, lsm, 1, 1);
    //crossImgPermutation(imgsInfo, randCoor, delsm, 1, 0);


    _imgTraversalInfo *imgTraInfo = (_imgTraversalInfo*)malloc(sizeof(_imgTraversalInfo));
    initialImgTraveralInfo(imgTraInfo, imgsInfo, imgNum);

    crossImgPermutation(imgsInfo, randCoor, lsm, 1, 0);
    crossImgForwardDiffusion(imgsInfo, imgTraInfo, &lscVal);
    crossImgBackwardDiffusion(imgsInfo, imgTraInfo, &lscVal1);


    deCrossImgBackwardDiffusion(imgsInfo, imgTraInfo, &delscVal1);
    deCrossImgForwardDiffusion(imgsInfo, imgTraInfo, &delscVal);
    crossImgPermutation(imgsInfo, randCoor, delsm, 1, 1);

//    lsm->x = 0.23487964;
//    lsm->y = 0.37894513;
//    delsm->x = 0.23487964;
//    delsm->y = 0.37894513;
//
//    lscVal = 0.54437346332574;
//    lscVal1 = 0.32437343432556;
//    delscVal = 0.54437346332574;
//    delscVal1 = 0.32437343432556;
//
//    crossImgPermutation(imgsInfo, randCoor, lsm, 1, 1);
//    crossImgForwardDiffusion(imgsInfo, imgTraInfo, &lscVal);
//    crossImgBackwardDiffusion(imgsInfo, imgTraInfo, &lscVal1);
//    deCrossImgBackwardDiffusion(imgsInfo, imgTraInfo, &delscVal1);
//    deCrossImgForwardDiffusion(imgsInfo, imgTraInfo, &delscVal);
//    crossImgPermutation(imgsInfo, randCoor, delsm, 1, 0);

    writeBatchImg(imgsInfo);

    free(lsm);
    free(delsm);
    freeRandCoor(randCoor);
    //freeRandCoor(deRandCoor);
    freeBatchImg(imgsInfo);
    return 0;
}
