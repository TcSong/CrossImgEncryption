#include "CrossImageDiffusion.h"
#include "Chaos.h"

void selectsort(int *a, int length) {
    int i = 0, j = 0, t = 0;
    for (i = 0; i <= length - 2; ++i) {
        for (j = i + 1; j <= length - 1; ++j) {
            if (a[i] > a[j]) {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
        }
    }
}

int countDiffLen(int *a, int length) {
    int i = 0, num = 1;
    for (i = 1; i < length; ) {
        if (a[i] != a[i - 1]) {
            ++num;
            ++i;
        }
        if (a[i] == a[i-1])
            ++i;
    }
    return num;
}

void countEachDiffLen(int *a, int *b, int a_len) {
    int i = 0, j = 0;
    b[j++] = a[0];
    for (i = 1; i < a_len; ) {
        if (a[i] != a[i-1]) {
            b[j++] = a[i];
            ++i;
        } else
            ++i;

    }
}

void freeImgTraveralInfo(_imgTraversalInfo *imgTraInfo) {
    free(imgTraInfo->imgIDset);
    free(imgTraInfo->eachDiffLen);
    free(imgTraInfo->imgLenset);
    free(imgTraInfo->LenCorrespondingImgID);
    free(imgTraInfo);
}


void initialImgTraveralInfo(_imgTraversalInfo *imgTraInfo, batchImgInfo *imgsInfo, int imgNum) {
    int i;

    imgTraInfo->imgIDset = (int*)malloc(sizeof(int) * imgNum);
    imgTraInfo->imgLenset = (int*)malloc(sizeof(int) * imgNum);

    imgTraInfo->arraySize = imgNum;     //还剩余的图像的数目  imgTraInfo->imgIDset[i] 来控制循环次数

    for (i = 0; i < imgNum; ++i) {
        imgTraInfo->imgIDset[i] = i;
        imgTraInfo->imgLenset[i] = imgsInfo[i].imglen;
    }
    selectsort(imgTraInfo->imgLenset, imgNum);
    imgTraInfo->numDiffLen = countDiffLen(imgTraInfo->imgLenset, imgNum);
//    for (i = 0; i < imgNum; ++i)
//        printf("%d ", imgTraInfo->imgLenset[i]);
    //printf("%d\n", imgTraInfo->numDiffLen);

    imgTraInfo->eachDiffLen = (int*)malloc(sizeof(int) * imgTraInfo->numDiffLen);        //然后赋值每一个len有多长

    countEachDiffLen(imgTraInfo->imgLenset, imgTraInfo->eachDiffLen, imgNum);

    for (i = 0; i < imgNum; ++i) {          //恢复成原来的样子
        imgTraInfo->imgIDset[i] = i;
        imgTraInfo->imgLenset[i] = imgsInfo[i].imglen;
    }
//    for (i = 0; i < imgTraInfo->numDiffLen; ++i)
//        printf("%d ", imgTraInfo->eachDiffLen[i]);

}

int resizeimgTraveralInfo(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthTodelete) {
    int i = 0, j = 0;
    int num = totalNum;
    for (i = 0; i < totalNum; ++i) {
        if (imgsInfo[ imgID[i] ].imglen == widthTodelete) {
            for (j = i; j < totalNum-1; ++j)
                imgID[j] = imgID[j+1];
            --totalNum;         //好多坑呀
            --num;
            --i;
        }
    }
    return num;
}



void crossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal) {
    int i = 0, j = 0, m = 0;
    int startLen = 0;

    int widthTodelete = 0;

    unsigned char preSeed = 123;
    unsigned char ks = 0;

    preiterLSCMap(lscVal, 500);
    //printf("%d\n", imgTraInfo->arraySize);

    for (i = 0; i < imgTraInfo->numDiffLen; ++i) {
        //在这里调整一下 IDSet  arraysize是剩余的图像数目
        if (i == 0)
            startLen = 0;
        else {
            startLen = imgTraInfo->eachDiffLen[i-1];
            widthTodelete = imgTraInfo->eachDiffLen[i-1];
            //printf("%d\n", widthTodelete);
            //printf("%d\n", imgTraInfo->arraySize);
            imgTraInfo->arraySize = resizeimgTraveralInfo(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthTodelete);
            //printf("%d\n", imgTraInfo->arraySize);
        }

        for (j = startLen; j < imgTraInfo->eachDiffLen[i]; ++j) {

            for (m = 0; m < imgTraInfo->arraySize; ++m) {           //imgIDset[ i ]已经是一个hash了

                LSCMap(lscVal, 1);
                ks = (long long)(*lscVal * 1.0e+15) & 255;

                if (m == 0) {
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] =
                    ks ^ ((imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] + ks) & 255) ^ preSeed;
                }
                if (m >= 1) {
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] =
                    ks ^ ((imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] + ks) & 255) ^ (imgsInfo[ imgTraInfo->imgIDset[m-1] ].imgData[j]);
                }

            }
            preSeed = imgsInfo[ imgTraInfo->imgIDset[m-1] ].imgData[j];
        }
    }
}

void deCrossImgForwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal) {
    int i = 0, j = 0, m = 0;
    int startLen = 0;

    int widthTodelete = 0;

    unsigned char preSeed = 123;
    unsigned char ks = 0;
    unsigned char pre_ciph_px = 0;
    unsigned char tmp = 0;

    for (i = 0; i < imgNum; ++i)           //恢复成原来的样子
        imgTraInfo->imgIDset[i] = i;
    imgTraInfo->arraySize = imgNum;

    preiterLSCMap(lscVal, 500);

    for (i = 0; i < imgTraInfo->numDiffLen; ++i) {
        if (i == 0)
            startLen = 0;
        else {
            startLen = imgTraInfo->eachDiffLen[i-1];
            widthTodelete = imgTraInfo->eachDiffLen[i-1];
            imgTraInfo->arraySize = resizeimgTraveralInfo(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthTodelete);
        }

        for (j = startLen; j < imgTraInfo->eachDiffLen[i]; ++j) {

            pre_ciph_px = imgsInfo[ imgTraInfo->imgIDset[0] ].imgData[j];

            for (m = 0; m < imgTraInfo->arraySize; ++m) {           //imgIDset[ i ]已经是一个hash了

                LSCMap(lscVal, 1);
                ks = (long long)(*lscVal * 1.0e+15) & 255;

                if (m == 0) {
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] = ((ks ^ imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] ^ preSeed) + 256 - ks) & 255;
                }
                if (m >= 1) {
                    tmp  = imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j];
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] = ((ks ^ imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] ^ pre_ciph_px) + 256 - ks) & 255;
                    pre_ciph_px = tmp;
                    preSeed = tmp;
                }

            }
            //preSeed = imgsInfo[ imgTraInfo->imgIDset[m-1] ].imgData[j];
        }
    }
}

int resizeimgTraveralInfoForBackwardDiffusion(batchImgInfo *imgsInfo, int *imgID, int totalNum, int widthToAdd) {
    int i = 0, j = 0;
    int num = totalNum;
    int tmp = 0;
    for (i = num; i < imgNum; ++i) {
        if (imgsInfo[ imgID[i] ].imglen == widthToAdd) {
            tmp = imgID[totalNum];
            imgID[totalNum] = imgID[i];
            imgID[i] = tmp;
            ++totalNum;
        }
    }
    selectsort(imgID, totalNum);
    return totalNum;
}


void crossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal) {
    int i = 0, j = 0, m = 0;
    int startLen = 0, endPos = 0;

    int widthToAdd = 0;

    unsigned char preSeed = 123;
    unsigned char ks = 0;

    for (i = 0; i < imgNum; ++i)           //恢复成原来的样子
        imgTraInfo->imgIDset[i] = i;
    imgTraInfo->arraySize = 0;

    preiterLSCMap(lscVal, 500);

    for (i = imgTraInfo->numDiffLen-1; i >= 0; --i) {
        //在这里调整一下 IDSet  arraysize是剩余的图像数目
        if (i == 0) {
            endPos = 0;
            startLen = imgTraInfo->eachDiffLen[i]-1;
            widthToAdd = imgTraInfo->eachDiffLen[i];
            imgTraInfo->arraySize = resizeimgTraveralInfoForBackwardDiffusion(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthToAdd);
        }
        else {
            endPos = imgTraInfo->eachDiffLen[i-1];
            startLen = imgTraInfo->eachDiffLen[i]-1;
            widthToAdd = imgTraInfo->eachDiffLen[i];
            imgTraInfo->arraySize = resizeimgTraveralInfoForBackwardDiffusion(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthToAdd);
        }


        for (j = startLen; j >= endPos; --j) {


            for (m = imgTraInfo->arraySize-1; m >= 0; --m) {           //imgIDset[ i ]已经是一个hash了
                LSCMap(lscVal, 1);
                ks = (long long)(*lscVal * 1.0e+15) & 255;

                if (m == imgTraInfo->arraySize-1) {
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] =
                    ks ^ ((imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] + ks) & 255) ^ preSeed;
                }
                if (m < imgTraInfo->arraySize-1) {

                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] =
                    ks ^ ((imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] + ks) & 255) ^ (imgsInfo[ imgTraInfo->imgIDset[m+1] ].imgData[j]);
                }

            }
            preSeed = imgsInfo[ imgTraInfo->imgIDset[m+1] ].imgData[j];
        }
    }
}


void deCrossImgBackwardDiffusion(batchImgInfo *imgsInfo, _imgTraversalInfo *imgTraInfo, double* lscVal) {
    int i = 0, j = 0, m = 0;
    int startLen = 0, endPos = 0;

    int widthToAdd = 0;

    unsigned char preSeed = 123;
    unsigned char ks = 0;
    unsigned char pre_ciph_px = 0;
    unsigned char tmp = 0;

    for (i = 0; i < imgNum; ++i)           //恢复成原来的样子
        imgTraInfo->imgIDset[i] = i;
    imgTraInfo->arraySize = 0;

    preiterLSCMap(lscVal, 500);

    for (i = imgTraInfo->numDiffLen-1; i >= 0; --i) {
        //在这里调整一下 IDSet  arraysize是剩余的图像数目
        if (i == 0) {
            endPos = 0;
            startLen = imgTraInfo->eachDiffLen[i]-1;
            widthToAdd = imgTraInfo->eachDiffLen[i];
            imgTraInfo->arraySize = resizeimgTraveralInfoForBackwardDiffusion(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthToAdd);
        }
        else {
            endPos = imgTraInfo->eachDiffLen[i-1];
            startLen = imgTraInfo->eachDiffLen[i]-1;
            widthToAdd = imgTraInfo->eachDiffLen[i];
            imgTraInfo->arraySize = resizeimgTraveralInfoForBackwardDiffusion(imgsInfo, imgTraInfo->imgIDset, imgTraInfo->arraySize, widthToAdd);
        }


        for (j = startLen; j >= endPos; --j) {
            pre_ciph_px = imgsInfo[ imgTraInfo->imgIDset[ imgTraInfo->arraySize-1 ] ].imgData[j];

            for (m = imgTraInfo->arraySize-1; m >= 0; --m) {           //imgIDset[ i ]已经是一个hash了
                LSCMap(lscVal, 1);
                ks = (long long)(*lscVal * 1.0e+15) & 255;

                if (m == imgTraInfo->arraySize-1) {
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] = ((ks ^ imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] ^ preSeed) + 256 - ks) & 255;
                }
                if (m < imgTraInfo->arraySize-1) {
                    tmp  = imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j];
                    imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] = ((ks ^ imgsInfo[ imgTraInfo->imgIDset[m] ].imgData[j] ^ pre_ciph_px) + 256 - ks) & 255;
                    pre_ciph_px = tmp;
                    preSeed = tmp;
                }
            }
            //preSeed = imgsInfo[ imgTraInfo->imgIDset[m+1] ].imgData[j];
        }
    }
}

