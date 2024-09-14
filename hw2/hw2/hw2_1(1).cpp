/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>
using namespace std;

int _11main()
{
	char  input_img[] = "lena256.raw";                 // Input  raw image name
	char  input_512img[] = "lena512.raw";                 // Input  raw image name
	char zoom_lena_to_512[] = "1(1)zoom_lena_to_512.raw";              // Output raw image name

	int width = 256;
	int height = 256;
	int size = width * height;

	unsigned char* img_lena = new unsigned char[size]; // array for原始256資料
	unsigned char* img_512_lena = new unsigned char[512 * 512]; // array for image data原始512資料
	unsigned char* my_img_lena = new unsigned char[512 * 512]; // array for image data 我生成的512

	int** darr_lena_512 = new int* [512];
	for (int i = 0; i < 512; i++) {
		darr_lena_512[i] = new int[512];
	}

	FILE* input_file;
	FILE* output_file;

	input_file = fopen(input_img, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	fread(img_lena, 1, size, input_file);

	for (int i = 0; i < 256; i += 1) {
		for (int j = 0; j < 256; j += 1) {
			darr_lena_512[2 * i][j * 2] = img_lena[i * 256 + j];
			darr_lena_512[2 * i][j * 2 + 1] = img_lena[i * 256 + j];
			darr_lena_512[2 * i + 1][j * 2] = img_lena[i * 256 + j];
			darr_lena_512[2 * i + 1][j * 2 + 1] = img_lena[i * 256 + j];
		}
	}
	
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			//轉回一維
			my_img_lena[i * 512 + j] = char(darr_lena_512[i][j]);
		}
	}

	output_file = fopen(zoom_lena_to_512, "wb");
	fwrite(my_img_lena, 1, 512 * 512, output_file);


	input_file = fopen(input_512img, "rb");
	fread(img_512_lena, 1, 512*512, input_file);

	double mse = 0.0;
	double  sum = 0.0;
	double psnr = 0.0;

	for (int i = 0; i < 512 * 512; i++) {
		double diff = img_512_lena[i] - my_img_lena[i]; // 計算對應像素的差
		sum += diff * diff; // 將差的平方累加到總和中
	}
	mse = sum / (512 * 512);
	psnr = 20 * log10(255 / sqrt(mse));
	cout << "1: 1_(1) mse = " << mse << endl;
	cout << "2: 1_(1) psnr = " << psnr << endl;;

	delete[] img_lena, my_img_lena, img_512_lena;

	for (int i = 0; i < 256; i++) {
		delete[] darr_lena_512[i];
	}
	delete[] darr_lena_512;
	fclose(input_file);
	fclose(output_file);
}
