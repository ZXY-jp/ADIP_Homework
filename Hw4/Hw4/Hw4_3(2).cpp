/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#define W 512
#define H 384
#define size  W*H
using namespace std;

int filter(int** img, float filter[3][3], int i, int j, int avg) {	//整張圖、filter、index
	int a = 0;
	//計算filter裡面總和
	for (int dy = -1; dy <= 1; dy++) {
		for (int dx = -1; dx <= 1; dx++) {
			int ny = i + dy;
			int nx = j + dx;
			// 檢查邊界，避免超出圖像範圍
			if (ny < 0) ny = 0;
			if (ny >= H) ny = H - 1;
			if (nx < 0) nx = 0;
			if (nx >= W) nx = W - 1;
			a += img[ny][nx] * filter[dy + 1][dx + 1];
		}
	}
	if (a / avg > 255)
		a = 255;
	else if (a / avg < 0)
		a = 0;
	else
		a = a / avg;

	return a;
}

void _32main() {
	char  input_img[] = "flower_512x384.raw";                 // Input  raw image name
	char  ROI_img[] = "3(1)ROI.raw";                 // Input  raw image name
	char  output_img[] = "3(2)highboot&gaussian.raw";                 // Input  raw image name

	FILE* input_file;
	FILE* output_file;

	int count = 1;
	unsigned char* img = new unsigned char[size]; // array for image data
	unsigned char* ROI_arr = new unsigned char[size]; //存放黑白 mask
	unsigned char* new_arr = new unsigned char[size]; //存放黑白 mask

	float gaussian3[3][3] = { {2,4,2},{4,8,4},{2,4,2} };
	float highboost_A[3][3] = { { -1,-1,-1 }, { -1,9.2,-1 }, {-1,-1,-1} };

	input_file = fopen(input_img, "rb");
	fread(img, 1, size, input_file);
	input_file = fopen(ROI_img, "rb");
	fread(ROI_arr, 1, size, input_file);


	int** img_darr = new int* [H];
	for (int i = 0; i < H; i++) {
		img_darr[i] = new int[W];
	}

	for (int i = 0; i < H; i++) {	//將原始轉為2維方便filter通過
		for (int j = 0; j < W; j++) {
			img_darr[i][j] = img[i * W + j];
		}
	}

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			int new_pexel = 0;
			if (ROI_arr[i * W + j] == 255) {	
				new_pexel = filter(img_darr, highboost_A, i, j, 1);
			}
			else {
				new_pexel = filter(img_darr, gaussian3, i, j, 32);
			}
			if (new_pexel > 255)
				new_arr[i * W + j] = 255;
			else if (new_pexel < 0)
				new_arr[i * W + j] = 0;
			else
				new_arr[i * W + j] = new_pexel;
		}
	}
	output_file = fopen(output_img, "wb");
	fwrite(new_arr, 1, size, output_file);
}