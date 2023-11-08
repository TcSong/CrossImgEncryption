#ifndef CROSSIMAGEDIFFUSION_H_INCLUDED
#define CROSSIMAGEDIFFUSION_H_INCLUDED

#include "ReadBatchImages.h"

typedef struct {
    int *imgIDset;      //图像ID集合 {0, 1, 2, 3, 4}
    int *imgLenset;     //图像长度结合,按照升序排列
    int *LenCorrespondingImgID;         //每个长度对应的图像ID
    int numDiffLen;         //多图像扩散时的循环次数
    int *eachDiffLen;
    int arraySize;
} _imgTraversalInfo;

void initialImgTraveralInfo(_imgTraversalInfo *imgTraInfo, batchImgInfo *imgsInfo, int imgNum);
void selectsort(int *a, int length);
int countDiffLen(int *a, int length);
void countEachDiffLen(int *a, int *b, int length);

int resizeimgTraveralInfo(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthTodelete);        //根据要删除的imglen删掉image编号，然后重置数组
int resizeimgTraveralInfoForBackwardDiffusion(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthToAdd);

void freeImgTraveralInfo(_imgTraversalInfo *imgTraInfo);

void crossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);
void deCrossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);

void crossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);
void deCrossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);


#endif // CROSSIMAGEDIFFUSION_H_INCLUDED
