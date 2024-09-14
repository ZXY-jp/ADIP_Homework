/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#define W 100
#define H 100
#define size  W*H

using namespace std;

double moment(double X, double Y, int p, int q, int f) {
	return pow(X, p) * pow(Y, q) * f;
}
void centroid(unsigned char* img, int count) {
	double m01 = 0;
	double m10 = 0;
	double m00 = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			m01 += img[i * W + j] * i;
			m10 += img[i * W + j] * j;
			m00 += img[i * W + j];
		}
	}

	double centroidX = m10 / m00;
	double centroidY = m01 / m00;
	double mu01 = 0;
	double mu10 = 0;
	double mu11 = 0;
	double mu02 = 0;
	double mu20 = 0;
	double mu12 = 0;
	double mu21 = 0;
	double mu03 = 0;
	double mu30 = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			mu01 += moment(j - centroidX, i - centroidY, 0, 1, img[i * W + j]);
			mu10 += moment(j - centroidX, i - centroidY, 1, 0, img[i * W + j]);
			mu11 += moment(j - centroidX, i - centroidY, 1, 1, img[i * W + j]);
			mu02 += moment(j - centroidX, i - centroidY, 0, 2, img[i * W + j]);
			mu20 += moment(j - centroidX, i - centroidY, 2, 0, img[i * W + j]);
			mu12 += moment(j - centroidX, i - centroidY, 1, 2, img[i * W + j]);
			mu21 += moment(j - centroidX, i - centroidY, 2, 1, img[i * W + j]);
			mu03 += moment(j - centroidX, i - centroidY, 0, 3, img[i * W + j]);
			mu30 += moment(j - centroidX, i - centroidY, 3, 0, img[i * W + j]);
		}
	}

	//cout <<"1(1): In shape_" << count << ".raw the centroid is: (" << centroidX << ", " << centroidY << ")" << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ01 is: " << mu01 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ10 is: " << mu10 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ11 is: " << mu11 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ02 is: " << mu02 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ20 is: " << mu20 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ12 is: " << mu12 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ21 is: " << mu21 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ03 is: " << mu03 << endl;
	cout  <<"1(2): "<< count << ": In shape_" << count << ".raw the μ30 is: " << mu30 << endl;
	cout << endl;
}


void _1main()
{
	char  input_img_1[] = "shape100_1.raw";                 // Input  raw image name
	char  input_img_2[] = "shape100_2.raw";                 // Input  raw image name
	char  input_img_3[] = "shape100_3.raw";                 // Input  raw image name
	char  input_img_4[] = "shape100_4.raw";                 // Input  raw image name
	char  input_img_5[] = "shape100_5.raw";                 // Input  raw image name
	char  input_img_6[] = "shape100_6.raw";                 // Input  raw image name
	char  input_img_7[] = "shape100_7.raw";                 // Input  raw image name
	char  input_img_8[] = "shape100_8.raw";                 // Input  raw image name

	FILE* input_file;

	int count = 1;
	unsigned char* img = new unsigned char[size]; // array for image data

	input_file = fopen(input_img_1, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_2, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_3, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_4, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_5, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_6, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_7, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	input_file = fopen(input_img_8, "rb");
	fread(img, 1, size, input_file);
	centroid(img, count);
	count++;

	delete[] img;
	fclose(input_file);
}
