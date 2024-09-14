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

void blur_filter(int** img, unsigned char* new_arr, float filter[3][3], int avg, char* filename) {
	FILE* output_file;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
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
			if (a / avg > 186)	//設定邊界值
				new_arr[i * W + j] = 255;
			else
				new_arr[i * W + j] = 0;
		}
	}
	output_file = fopen(filename, "wb");
	fwrite(new_arr, 1, size, output_file);
}

void _31main() {
	char  input_img[] = "flower_512x384.raw";                 // Input  raw image name
	char  output_ROI_img[] = "3(1)ROI.raw";                 // Input  raw image name

	FILE* input_file;

	int count = 1;
	unsigned char* img = new unsigned char[size]; // array for image data
	unsigned char* new_arr = new unsigned char[size]; //存放結果

	float gaussian3[3][3] = { {1,2,1},{2,4,2},{1,2,1} };

	input_file = fopen(input_img, "rb");
	fread(img, 1, size, input_file);

	int** img_darr = new int* [H];
	for (int i = 0; i < H; i++) {
		img_darr[i] = new int[W];
	}

	for (int i = 0; i < H; i++) {	//將原始轉為2維方便filter通過
		for (int j = 0; j < W; j++) {
			img_darr[i][j] = img[i * W + j];
		}
	}
	blur_filter(img_darr, new_arr, gaussian3, 16, output_ROI_img);
}