#ifndef READBATCHIMAGES_H_INCLUDED
#define READBATCHIMAGES_H_INCLUDED

#include <stdio.h>
#include <sys/types.h>
#include <stddef.h>
#include <dirent.h>
#include <windows.h>

#define Max_File_Num 1024
#define File_Name_Len 255

extern char imgNames[Max_File_Num][File_Name_Len];          //�洢����ͼ�������
extern char outputImgNames[Max_File_Num][File_Name_Len];
extern int imgNum;                                          //ͼ�������

//ͼ��ͷ�ļ���Ϣ
typedef struct {
	unsigned short bfType;			//�ļ���ʶ��Ϊ��ĸASCII�롰BM��
	int bfSize;				//λͼ�ļ���С�����ֽ�Ϊ��λ
	short int bfReserved1;  //λͼ�ļ������֣�����Ϊ0
	short int bfReserved2;	//λͼ�ļ������֣�����Ϊ0
	int bfOffbits;			//�ļ���ʼ��λͼ���鿪ʼ֮���ƫ�����ֽ�
} FILEHEADER;							//�ýṹ��ռ��14���ֽ�

typedef struct{
	int biSize;				//���ṹ����ռ���ֽ���
	int biWidth;			//λͼ�Ŀ�ȣ�������Ϊ��λ
	int biHeight;			//λͼ�ĸ߶ȣ�������Ϊ��λ
	short biPlanes;			//Ŀ���豸��ƽ����������Ϊ1
	short biBitCount;		//ÿ�����������λ����������1(˫ɫ),4(16)ɫ��8(256)ɫ��24(���ɫ)
	int biCompression;		//λͼѹ�����ͣ�������0(��ѹ��),1(BI_RLEBѹ������)��2(BI_REL4ѹ������)֮һ
	int biSizeImage;		//λͼ�ļ���С�����ֽ�Ϊ��λ
	int biXPelsPerMeter;	//λͼˮƽ�ֱ��ʣ�ÿ��������
	int biYPelsPerMeter;	//λͼ��ֱ�ֱ��ʣ�ÿ��������
	int biClrUsed;			//λͼʵ��ʹ�õ���ɫ���е���ɫ��
	int biClrImporttant;	//λͼ��ʾ��������Ҫ����ɫ��
} INFOHEADER;							//�ýṹ��ռ��40���ֽ�

typedef struct {          //��ɫ��
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
