#include "ReadBatchImages.h"

char imgNames[Max_File_Num][File_Name_Len] = {{'\0'}};
char outputImgNames[Max_File_Num][File_Name_Len] = {{'\0'}};
int imgNum = 0;

int readBatchImages(char* imgsPath) {

    int i = 0;
    DWORD fileattr;
    DIR* drstreamob;                            //目录流对象
    struct dirent* dirp;                        //指向目录具体对象
    drstreamob = opendir(imgsPath);              //返回参数所指的目录流对象
    while ((dirp = readdir(drstreamob))) {        //读取目录流对象
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)      //跳过..和.
            continue;
        fileattr = GetFileAttributes(dirp->d_name);                //这是一个目录的话

        strcpy(imgNames[i], imgsPath);                      //先赋值路径
        strcpy(outputImgNames[i], imgsPath);
        strcat(outputImgNames[i], "CipherImages\\");
        mkdir(outputImgNames[i]);

        strcat(imgNames[i], dirp->d_name);                 //赋值文件名
        strcat(outputImgNames[i], dirp->d_name);            //输出的文件名

        fileattr = GetFileAttributes(imgNames[i]);
        if (fileattr & FILE_ATTRIBUTE_DIRECTORY) {
            memset(imgNames[i], 0, File_Name_Len);
            memset(outputImgNames[i], 0, File_Name_Len);
            continue;
        }
        ++i;
    }
    return i;
}

void loadBatchImageInfo(batchImgInfo* imgsInfo) {
    int i = 0, j = 0, m = 0, k = 0;
    FILE *fp[imgNum];
    int colormap_size = 0;
    int padding = 0;
    //rgb_quad* quad;

    FILEHEADER filehead;
    INFOHEADER fileInfo;
    //imgsInfo = (batchImgInfo*)malloc(sizeof(batchImgInfo) * imgNum);

    for (i = 0; i < imgNum; ++i) {
        k = 0;
        if ((fp[i] = fopen(imgNames[i], "rb")) == NULL) {
            printf("File open error...\n");
            return;
        }
        printf("%s\n", imgNames[i]);
        fread(&filehead, 1, 14, fp[i]);
        fread(&fileInfo, 1, 40, fp[i]);        //读取文件头和信息头

        int pos = 0;

        switch (fileInfo.biBitCount) {

            case 8:
                imgsInfo[i].imgData = (unsigned char*)malloc(sizeof(unsigned char) * imgsInfo[i].imglen);

                padding = 4 - fileInfo.biWidth % 4;
                if (padding == 4)
                    padding = 0;

                unsigned char temp;
                unsigned char *y = (unsigned char*)malloc(sizeof(unsigned char) * fileInfo.biWidth * fileInfo.biHeight);

                for (j = 0; j < fileInfo.biHeight; ++j) {
                    for (m = 0; m < fileInfo.biWidth; ++m) {
                        fread((char*)&temp, 1, 1, fp[i]);
                        pos = (fileInfo.biHeight - j - 1) * fileInfo.biWidth + m;
                        y[pos] = temp;
                        imgsInfo[i].imgData[k++] = y[pos];
                    }
                    fseek(fp[i], padding, SEEK_CUR);
                }

                imgsInfo[i].width = fileInfo.biWidth;
                imgsInfo[i].height = fileInfo.biHeight;
                imgsInfo[i].imglen = imgsInfo[i].width * imgsInfo[i].height;
                imgsInfo[i].bitCounts = 8;
                colormap_size = 4 * 256;
                free(y);
                break;
            case 24:
                imgsInfo[i].imglen = 3 * fileInfo.biWidth * fileInfo.biHeight;
                imgsInfo[i].imgData = (unsigned char*)malloc(sizeof(unsigned char) * imgsInfo[i].imglen);
                imgsInfo[i].bitCounts = 8;

                padding = 4 - (fileInfo.biWidth * 3) % 4;
                if (padding == 4)
                    padding = 0;
                unsigned char* Rdata = (unsigned char*)malloc( sizeof(unsigned char) * (2* fileInfo.biWidth * fileInfo.biHeight) );
                unsigned char* Gdata = (unsigned char*)malloc( sizeof(unsigned char) * (2* fileInfo.biWidth * fileInfo.biHeight) );
                unsigned char* Bdata = (unsigned char*)malloc( sizeof(unsigned char) * (2* fileInfo.biWidth * fileInfo.biHeight) );

                rgbData temp2;
                for (j = 0; j < fileInfo.biHeight; ++j) {
                    for (m = 0; m < fileInfo.biWidth; ++m) {
                    fread((char*)&temp2, 1, 3, fp[i]);
                        pos = (fileInfo.biHeight - j - 1) * fileInfo.biWidth + m;
                        Rdata[pos] = temp2.red;
                        Gdata[pos] = temp2.green;
                        Bdata[pos] = temp2.blue;
                        imgsInfo[i].imgData[k++] = Rdata[pos];
                        imgsInfo[i].imgData[k++] = Gdata[pos];
                        imgsInfo[i].imgData[k++] = Bdata[pos];
                    }
                    fseek(fp[i], padding, SEEK_CUR);
                }

                free(Rdata);
                free(Gdata);
                free(Bdata);

                imgsInfo[i].width = fileInfo.biWidth * 3;
                imgsInfo[i].height = fileInfo.biHeight;
                imgsInfo[i].imglen = imgsInfo[i].width * imgsInfo[i].height;
                colormap_size = 0;
                break;
            default:
                break;
        }

        imgsInfo[i].offbits = colormap_size + 54;
        imgsInfo[i].imgHeader = (unsigned char*)malloc(sizeof(unsigned char) * imgsInfo[i].offbits);


        fseek(fp[i], 0, SEEK_SET);          //把文件指针复位到起始位置
        fread(imgsInfo[i].imgHeader, 1, imgsInfo[i].offbits, fp[i]);
        //fread(imgsInfo[i].imgData, 1, imgsInfo[i].imglen, fp[i]);

        fclose(fp[i]);
    }
}

void writeBatchImg(batchImgInfo* imgsInfo) {
    int i = 0, j = 0, m = 0, k = 0;
    FILE *fp[imgNum];
    int padding = 0, filepadding = 0, offset = 0;
    int width = 0, height = 0;
    rgb_quad* quad;
    unsigned char numnull = 0;

    for (i = 0; i < imgNum; ++i) {
        FILEHEADER head;
        INFOHEADER info;

        if ((fp[i] = fopen(outputImgNames[i], "wb")) == NULL) {
            printf("Error written error...\n");
            return;
        }
        //printf("%d %d %d %d\n", imgsInfo[i].offbits, imgsInfo[i].height, imgsInfo[i].width, imgsInfo[i].imglen);

        head.bfType = 0x4d42;

        switch (imgsInfo[i].bitCounts) {
            case 24:
                width = imgsInfo[i].width / 3;
                height = imgsInfo[i].height;
                padding = 4 - (width * 3) % 4;
                if (padding == 4)
                    padding = 0;
                filepadding = 4 - ((width + padding)* height * 3) % 4;
                if (filepadding == 4)
                    filepadding = 0;

                head.bfSize = 54 + ( (sizeof(rgbData)) * width + padding ) * height + filepadding;
                head.bfReserved2 = head.bfReserved1 = 0;
                head.bfOffbits = 54;
                info.biBitCount = imgsInfo[i].bitCounts;
                info.biClrImporttant = 0;
                info.biClrUsed = 0;
                info.biCompression = 0;
                info.biHeight = height;
                info.biWidth = width;
                info.biPlanes = 1;
                info.biSize = 40;
                info.biSizeImage = head.bfSize - 54;
                info.biXPelsPerMeter = 0;
                info.biYPelsPerMeter = 0;
                break;
            case 8:
                width = imgsInfo[i].width;
                height = imgsInfo[i].height;

                padding = 4 - (width) % 4;
                if (padding == 4)
                    padding = 0;
                filepadding = 4 - ((width + padding)* height) % 4;
                if (filepadding == 4)
                    filepadding = 0;

                quad = (rgb_quad*)malloc(256 * sizeof(rgb_quad));
                for (m = 0; m < 256; ++m) {
                    quad[m].rgbBlue = m;
                    quad[m].rgbGreen = m;
                    quad[m].rgbRed = m;
                    quad[m].rgbReserved = 0;
                }
                int Quad_size = 256 * sizeof(rgb_quad);
                head.bfSize = 54 + Quad_size + (width + padding) * height + filepadding;
                head.bfReserved2 = head.bfReserved1 = 0;
                head.bfOffbits = 54 + Quad_size;
                info.biBitCount = imgsInfo[i].bitCounts;
                info.biClrImporttant = 0;
                info.biClrUsed = 0;
                info.biCompression = 0;
                info.biHeight = height;
                info.biWidth = width;
                info.biPlanes = 1;
                info.biSize = 40;
                info.biSizeImage = head.bfSize - 54 - Quad_size;
                info.biXPelsPerMeter = 0;
                info.biYPelsPerMeter = 0;
                break;
        }
        fwrite(imgsInfo[i].imgHeader, 1, imgsInfo[i].offbits, fp[i]);

//        if (info.biBitCount == 8) {
//			int size = 256 * sizeof(rgb_quad);
//			//printf("%d\n", sizeof(unsigned char));
//			//fwrite((char*)quad, 1, size, fp[i]);            //写入调色板
//		}

        switch (info.biBitCount) {
            case 24:
                for (j = 0; j < height; j++) {
                    offset = j * width;
                    unsigned char *data_pointer = imgsInfo[i].imgData + offset;
                    fwrite(data_pointer, 1, sizeof(unsigned char)*width*3, fp[i]);
                    for (m = padding; m > 0; m--)
                        fwrite(&numnull, 1, sizeof(unsigned char), fp[i]);
                }
                break;
            case 8:
                for (j = 0; j < height; ++j) {
                    offset = j * width;
                    unsigned char *data_pointer = imgsInfo[i].imgData + offset;
                    fwrite(data_pointer, 1, sizeof(unsigned char)*width, fp[i]);
                    for (m = padding; m > 0; m--)
                        fwrite(&numnull, 1, sizeof(unsigned char), fp[i]);
                }
                break;
        }
        for (m = 0; m < filepadding; m++)
            fwrite(&numnull, 1, sizeof(unsigned char), fp[i]);
        //fwrite(imgsInfo[i].imgData, 1, imgsInfo[i].offbits, fp[i]);

        fclose(fp[i]);
    }
}

void freeBatchImg(batchImgInfo* imgsInfo) {
    int i;
    for (i = 0; i < imgNum; ++i)
        free(imgsInfo[i].imgData);
    free(imgsInfo);
}
