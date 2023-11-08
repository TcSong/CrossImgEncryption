#ifndef READBATCHIMAGES_H_INCLUDED
#define READBATCHIMAGES_H_INCLUDED

#include <stdio.h>
#include <sys/types.h>
#include <stddef.h>
#include <dirent.h>
#include <windows.h>

#define Max_File_Num 1024
#define File_Name_Len 255

extern char imgNames[Max_File_Num][File_Name_Len];          //存储所有图像的名字
extern char outputImgNames[Max_File_Num][File_Name_Len];
extern int imgNum;                                          //图像的总数

//图像头文件信息
typedef struct {
	unsigned short bfType;			//文件标识，为字母ASCII码“BM”
	int bfSize;				//位图文件大小，以字节为单位
	short int bfReserved1;  //位图文件保留字，必须为0
	short int bfReserved2;	//位图文件保留字，必须为0
	int bfOffbits;			//文件开始到位图数组开始之间的偏移量字节
} FILEHEADER;							//该结构体占据14个字节

typedef struct{
	int biSize;				//本结构体所占据字节数
	int biWidth;			//位图的宽度，以像素为单位
	int biHeight;			//位图的高度，以像素为单位
	short biPlanes;			//目标设备的平面数，必须为1
	short biBitCount;		//每个像素所需的位数，必须是1(双色),4(16)色，8(256)色，24(真彩色)
	int biCompression;		//位图压缩类型，必须是0(不压缩),1(BI_RLEB压缩类型)或2(BI_REL4压缩类型)之一
	int biSizeImage;		//位图文件大小，以字节为单位
	int biXPelsPerMeter;	//位图水平分辨率，每米像素数
	int biYPelsPerMeter;	//位图垂直分辨率，每米像素数
	int biClrUsed;			//位图实际使用的颜色表中的颜色数
	int biClrImporttant;	//位图显示过程中重要的颜色数
} INFOHEADER;							//该结构体占据40个字节

typedef struct {          //调色板
       unsigned char rgbBlue;
       unsigned char rgbGreen;
       unsigned char rgbRed;
       unsigned char rgbReserved;
}rgb_quad;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} rgbData;

typedef struct {
    int width;
    int height;
    int imglen;
    int offbits;
    int bitCounts;
    unsigned char* imgHeader;
    unsigned char* imgData;
} batchImgInfo;


int readBatchImages(char* imgPath);

void loadBatchImageInfo(batchImgInfo* imgsInfo);
void writeBatchImg(batchImgInfo* imgsInfo);

void freeBatchImg(batchImgInfo* imgsInfo);



#endif // READBATCHIMAGES_H_INCLUDED
