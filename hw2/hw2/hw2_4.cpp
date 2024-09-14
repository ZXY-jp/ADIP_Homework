/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<ctime>
#define _CRT_SECURE_NO_WARNINGS

using namespace cv;
using namespace std;

int _4main()
{
	int width = 1024;
	int height = 1024;
	int size = width * height;

	//建立畫布，Scalar 設定色彩(BGR)
	char  input_img[] = "lena1024.raw";                 // Input  raw image name
	char output_img[] = "4.lena1024_halftone.raw";              // Output bright raw image name
	FILE* input_file;
	FILE* output_file;

	unsigned char* img_lena = new unsigned char[size]; // array for image data
	unsigned char* img_lena_halftone = new unsigned char[size]; // array for image data

	errno_t  err;
	err = fopen_s(&input_file, input_img, "rb");
	if (err != 0) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	int** darr_lena = new int* [1024];
	int** darr_lena_halftone = new int* [1024];

	for (int i = 0; i < 1024; i++) {
		darr_lena[i] = new int[1024];
		darr_lena_halftone[i] = new int[1024];
	}

	fread(img_lena, 1, size, input_file);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			darr_lena[i][j] = img_lena[i * width + j];
		}
	}

	/*---------------------------------------------------------------------*/
	Mat image(width, height, CV_8UC3, Scalar(255,255,255));
	int radious = 5;

	for (int i = 0; i < height; i += 16) {
		for (int j = 0; j < width; j += 16) {
			int avg = 0;
			int sum = 0;
			//計算16*16中的avg
			for (int m = i; m < i + 16; m++) {
				for (int n = j; n < j + 16; n++) {
					sum += darr_lena[i][j];
				}
			}
			avg = sum / (16 * 16);

			if (avg < 58) {
				radious = 8;
			}
			else if (avg < 81) {
				radious = 7;
			}
			else if (avg < 105) {
				radious = 6;
			}
			else if (avg < 129) {
				radious = 5;
			}
			else if (avg < 153)
			{
				radious = 4;
			}
			else if (avg < 177) {
				radious = 3;
			}
			else if (avg < 201) {
				radious = 2;
			}
			else if (avg < 255) {
				radious = 1;
			}

			for (int m = i; m < i + 16; m++) {
				for (int n = j; n < j + 16; n++) {
					if ((8 - radious) * 31 > 255)
						darr_lena_halftone[m][n] = 255;
					else if((8 - radious) *31 <0)
						darr_lena_halftone[m][n] = 0;
					else
						darr_lena_halftone[m][n] = (8 - radious) * 31;
				}
			}
			Point c(j + 8, i + 8);
			circle(image, c, radious, Scalar(0, 0, 0), -1);	// 點點
			img_lena_halftone[i * 1024 + j] = int(image.at<uchar>(i, j));
		}
	}
	

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//轉回一維
			img_lena_halftone[i * 1024 + j] = char(darr_lena_halftone[i][j]);
		}
	}

	err = fopen_s(&output_file, output_img, "wb");
	fwrite(img_lena_halftone, 1, 1024 * 1024, output_file);	

	imshow("halftone lena", image);
	imwrite("4.halftone_lena.png", image);	//儲存結果為png
	waitKey(0);


	delete[]img_lena, img_lena_halftone;
	for (int i = 0; i < 1024; i++) {
		delete[] darr_lena[i];
		delete[] darr_lena_halftone[i];
	}
	delete[] darr_lena, darr_lena_halftone;

	fclose(input_file);
	fclose(output_file);

	destroyAllWindows();
	return 0;
}
