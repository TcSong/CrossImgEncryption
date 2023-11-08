#include "CrossImagePermutation.h"

void crossImgPermutation(batchImgInfo *imgsInfo, _randCoor *randCoor, _2dLSM *lsm, int round, int flag) {
    int i = 0, j = 0;
    float ximg, ypos;
    //int tmprImg = 0, tmprPos;
    unsigned char tmpPos;

    if (round == 1)
        preiterChaos2DLSM(lsm, 1000);

    for (i = 0; i < imgNum; ++i) {
        randCoor[i].randImg = (int*)malloc(sizeof(int) * imgsInfo[i].imglen);
        randCoor[i].randPos = (int*)malloc(sizeof(int) * imgsInfo[i].imglen);

        for (j = 0; j < imgsInfo[i].imglen - 1; ++j) {
            chaos2DLSM(lsm, 1);
            ximg = fabs(lsm->x);
            ypos = fabs(lsm->y);

            while (ximg > 1)
                ximg /= 10;
            ximg *= (1.0e+7);


            randCoor[i].randImg[j] = i + (int)ximg % (imgNum - i);

            while (ypos > 1)
                ypos /= 10;
            ypos *= (1.0e+7);

            if (randCoor[i].randImg[j] == i)
                randCoor[i].randPos[j] = j + (1 + ((int)ypos)%(imgsInfo[i].imglen-1-j) );

            if (randCoor[i].randImg[j] != i)
                randCoor[i].randPos[j] = ((int)ypos)%imgsInfo[ randCoor[i].randImg[j] ].imglen;
        }
        chaos2DLSM(lsm, 1);
        ximg = fabs(lsm->x);
        ypos = fabs(lsm->y);
        while (ximg > 1)
            ximg /= 10;
        ximg *= (1.0e+7);

        while (ypos > 1)
            ypos /= 10;
        ypos *= (1.0e+7);

//        printf("asdf");
//        getchar();
        if (i < imgNum-1) {             //最后一个点特殊处理
            randCoor[i].randImg[j] = i + 1 + (int)ximg % (imgNum - i - 1);
            randCoor[i].randPos[j] = ((int)ypos)%imgsInfo[ randCoor[i].randImg[j] ].imglen;
        } else {
            randCoor[i].randImg[j] = imgNum - 1;
            randCoor[i].randPos[j] = imgsInfo[randCoor[i].randImg[j]].imglen - 1;
        }
    }

    if (flag) {
        for (i = 0; i < imgNum; ++i) {
            for (j = 0; j < imgsInfo[i].imglen; ++j) {
                tmpPos = imgsInfo[i].imgData[j];
                imgsInfo[i].imgData[j] = imgsInfo[ randCoor[i].randImg[j] ].imgData[ randCoor[i].randPos[j] ];
                imgsInfo[ randCoor[i].randImg[j] ].imgData[ randCoor[i].randPos[j] ] = tmpPos;
            }
        }
    } else {
        for (i = imgNum-1; i >= 0; --i) {
            for (j = imgsInfo[i].imglen-1; j >= 0; --j) {
                tmpPos = imgsInfo[i].imgData[j];
                imgsInfo[i].imgData[j] = imgsInfo[ randCoor[i].randImg[j] ].imgData[ randCoor[i].randPos[j] ];
                imgsInfo[ randCoor[i].randImg[j] ].imgData[ randCoor[i].randPos[j] ] = tmpPos;
            }
        }
    }
}

void deCrossImgPermutation(batchImgInfo *imgsInfo, _randCoor *deRandCoor, _2dLSM *delsm, int round){
    int i = 0, j = 0;
    float ximg, ypos;
    unsigned char tmpPos;

    if (round == 1)
        preiterChaos2DLSM(delsm, 500);

    for (i = 0; i < imgNum; ++i) {
        deRandCoor[i].randImg = (int*)malloc(sizeof(int) * imgsInfo[i].imglen);
        deRandCoor[i].randPos = (int*)malloc(sizeof(int) * imgsInfo[i].imglen);

        for (j = 0; j < imgsInfo[i].imglen - 1; ++j) {
            chaos2DLSM(delsm, 1);
            ximg = fabs(delsm->x);
            ypos = fabs(delsm->y);

            while (ximg > 1)
                ximg /= 10;
            ximg *= (1.0e+7);

            deRandCoor[i].randImg[j] = i + (int)ximg % (imgNum - i);

            while (ypos > 1)
                ypos /= 10;
            ypos *= (1.0e+7);

            if (deRandCoor[i].randImg[j] == i)
                deRandCoor[i].randPos[j] = j + (1 + ((int)ypos)%(imgsInfo[i].imglen-1-j) );

            if (deRandCoor[i].randImg[j] != i)
                deRandCoor[i].randPos[j] = ((int)ypos)%imgsInfo[ deRandCoor[i].randImg[j] ].imglen;
        }
        chaos2DLSM(delsm, 1);
        ximg = fabs(delsm->x);
        ypos = fabs(delsm->y);
        while (ximg > 1)
            ximg /= 10;
        ximg *= (1.0e+7);

        if (i < imgNum-1) {
            deRandCoor[i].randImg[j] = i+1 + (int)ximg % (imgNum - i-1);
            deRandCoor[i].randPos[j] = j + (1 + ((int)ypos)%(imgsInfo[i].imglen-1-j) );
        } else {
            deRandCoor[i].randImg[j] = imgNum - 1;
            deRandCoor[i].randPos[j] = imgsInfo[i].imglen - 1;
        }
    }
    for (i = imgNum - 1; i >=0; --i) {
        for (j = imgsInfo[i].imglen - 1; j >= 0; --j) {
            tmpPos = imgsInfo[i].imgData[j];
            imgsInfo[i].imgData[j] = imgsInfo[ deRandCoor[i].randImg[j] ].imgData[ deRandCoor[i].randPos[j] ];
            imgsInfo[ deRandCoor[i].randImg[j] ].imgData[ deRandCoor[i].randPos[j] ] = tmpPos;
        }
    }
}

void freeRandCoor(_randCoor *randCoor) {
    int i;
    for (i = 0; i < imgNum; ++i) {
        free(randCoor[i].randImg);
        free(randCoor[i].randPos);
    }
    free(randCoor);
}
