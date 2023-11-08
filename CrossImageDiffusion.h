#ifndef CROSSIMAGEDIFFUSION_H_INCLUDED
#define CROSSIMAGEDIFFUSION_H_INCLUDED

#include "ReadBatchImages.h"

typedef struct {
    int *imgIDset;      //ͼ��ID���� {0, 1, 2, 3, 4}
    int *imgLenset;     //ͼ�񳤶Ƚ��,������������
    int *LenCorrespondingImgID;         //ÿ�����ȶ�Ӧ��ͼ��ID
    int numDiffLen;         //��ͼ����ɢʱ��ѭ������
    int *eachDiffLen;
    int arraySize;
} _imgTraversalInfo;

void initialImgTraveralInfo(_imgTraversalInfo *imgTraInfo, batchImgInfo *imgsInfo, int imgNum);
void selectsort(int *a, int length);
int countDiffLen(int *a, int length);
void countEachDiffLen(int *a, int *b, int length);

int resizeimgTraveralInfo(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthTodelete);        //����Ҫɾ����imglenɾ��image��ţ�Ȼ����������
int resizeimgTraveralInfoForBackwardDiffusion(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthToAdd);

void freeImgTraveralInfo(_imgTraversalInfo *imgTraInfo);

void crossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);
void deCrossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);

void crossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);
void deCrossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal);


#endif // CROSSIMAGEDIFFUSION_H_INCLUDED
