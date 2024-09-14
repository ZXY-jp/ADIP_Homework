/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將_符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#define W 700
#define H 421
#define size  W*H
using namespace std;

void movefilter5x5(int** img, unsigned char* new_arr, double filter[5][5], int avg, char* filename) {
	FILE* output_file;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			int a = 0;
			//計算filter裡面總和
			for (int dy = -2; dy <= 2; dy++) {
				for (int dx = -2; dx <= 2; dx++) {
					int ny = i + dy;
					int nx = j + dx;
					// 檢查邊界，避免超出圖像範圍
					if (ny < 0) ny = 0;
					if (ny >= H) ny = H - 1;
					if (nx < 0) nx = 0;
					if (nx >= W) nx = W - 1;
					a += img[ny][nx] * filter[dy + 2][dx + 2];
				}
			}
			new_arr[i * W + j] = a / avg;
		}
	}
	output_file = fopen(filename, "wb");
	fwrite(new_arr, 1, size, output_file);
}

void movefilter3x3(int** img, unsigned char* new_arr, double filter[3][3], int avg, char* filename) {
	FILE* output_file;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			double a = 0;
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
				new_arr[i * W + j] = 255;
			else if (a / avg < 0)
				new_arr[i * W + j] = 0;
			else
				new_arr[i * W + j] = a / avg;
		}
	}
	output_file = fopen(filename, "wb");
	fwrite(new_arr, 1, size, output_file);
}


void main()
{
	char  input_img1[] = "slate_700x421.raw";				// Input  raw image name
	char  input_img2[] = "slate_noise_700x421.raw";         // Input  raw image name
	char  output_img1[] = "2(1)box_3x3.raw";
	char  output_img2[] = "2(1)box_5x5.raw";
	char  output_img3[] = "2(1)gaussian_3x3.raw";
	char  output_img4[] = "2(1)gaussian_5x5.raw";

	char  output_img5[] = "2(2)robert_45.raw";
	char  output_img6[] = "2(2)robert_-45.raw";
	char  output_img7[] = "2(2)robert_mix.raw";

	char  output_img8[] = "2(3)prewitt_0.raw";
	char  output_img9[] = "2(3)prewitt_90.raw";
	char  output_img10[] = "2(3)prewitt_mix.raw";

	char  output_img11[] = "2(4)sobel_0.raw";
	char  output_img12[] = "2(4)sobel_90.raw";
	char  output_img13[] = "2(4)sobel_mix.raw";

	char  output_img14[] = "2(5)sobel_-45.raw";
	char  output_img15[] = "2(5)sobel_45.raw";

	char  output_img16[] = "2(6)laplacian4.raw";
	char  output_img17[] = "2(6)laplacian8.raw";
	char  output_img18[] = "2(8)high boost A15.raw";
	char  output_img19[] = "2(8)high boost A3.raw";

	/*------------------------------------------------------*/
	char  output_noise_img1[] = "2(1)noise_box_3x3.raw";
	char  output_noise_img2[] = "2(1)noise_box_5x5.raw";
	char  output_noise_img3[] = "2(1)noise_gaussian_3x3.raw";
	char  output_noise_img4[] = "2(1)noise_gaussian_5x5.raw";

	char  output_noise_img5[] = "2(2)noise_robert_45.raw";
	char  output_noise_img6[] = "2(2)noise_robert_-45.raw";
	char  output_noise_img7[] = "2(2)noise_robert_mix.raw";

	char  output_noise_img8[] = "2(3)noise_prewitt_0.raw";
	char  output_noise_img9[] = "2(3)noise_prewitt_90.raw";
	char  output_noise_img10[] = "2(3)noise_prewitt_mix.raw";

	char  output_noise_img11[] = "2(4)noise_sobel_0.raw";
	char  output_noise_img12[] = "2(4)noise_sobel_90.raw";
	char  output_noise_img13[] = "2(4)noise_sobel_mix.raw";

	char  output_noise_img14[] = "2(5)noise_sobel_-45.raw";
	char  output_noise_img15[] = "2(5)noise_sobel_45.raw";
	char  output_noise_img16[] = "2(6)noise_laplacian4.raw";
	char  output_noise_img17[] = "2(6)noise_laplacian8.raw";
	char  output_noise_img18[] = "2(8)noise_high boost A15.raw";
	char  output_noise_img19[] = "2(8)noise_high boost A3.raw";

	FILE* input_file;
	FILE* output_file;

	int count = 1;
	unsigned char* img = new unsigned char[size]; // array for image data
	unsigned char* img_A = new unsigned char[size]; // array for image data
	unsigned char* img_B = new unsigned char[size]; // array for image data

	unsigned char* new_arr = new unsigned char[size]; //存放結果
	//各種filter
	double box3[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
	double box5[5][5] = { {1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1} };
	double gaussian3[3][3] = { {1,2,1},{2,4,2},{1,2,1} };
	double gaussian5[5][5] = { {1,4,6,4,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,4,1} };
	double robert45[3][3] = { {0,0,0}, {0,-1,0},{0,0,1} };
	double robert045[3][3] = { {0,0,0}, {0,0,-1},{0,1,0} };
	double prewitt0[3][3] = { { -1,-1,-1 }, { 0,0,0 }, {1,1,1} };
	double prewitt90[3][3] = { { -1,0,1 }, { -1,0,1 }, {-1,0,1} };
	double sobel0[3][3] = { { -1,-2,-1 }, { 0,0,0 }, {1,2,1} };
	double sobel90[3][3] = { { -1,0,1 }, { -2,0,2 }, {-1,0,1} };
	double sobel045[3][3] = { { 2,1,0 }, {1,0,-1 }, {0,-1,-2} };
	double sobel45[3][3] = { { -2,-1,0 }, {-1,0,1 }, {0,1,2} };
	double laplacian4[3][3] = { { 0,-1,0}, { -1,4,-1 }, {0,-1,0} };
	double laplacian8[3][3] = { { -1,-1,-1 }, { -1,8,-1 }, {-1,-1,-1} };
	double highboost_A15[3][3] = { { -1,-1,-1 }, { -1,9.5,-1 }, {-1,-1,-1} };
	double highboost_A3[3][3] = { { -1,-1,-1 }, { -1,11,-1 }, {-1,-1,-1} };


	/*----------------------------normal-----------------------------------*/

	input_file = fopen(input_img1, "rb");
	fread(img, 1, size, input_file);

	int** img_darr = new int* [H];	//將原始轉為2維方便filter通過
	for (int i = 0; i < H; i++) {
		img_darr[i] = new int[W];
	}

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			img_darr[i][j] = img[i * W + j];
		}
	}

	movefilter3x3(img_darr, new_arr, box3, 9, output_img1);
	movefilter5x5(img_darr, new_arr, box5, 25, output_img2);
	movefilter3x3(img_darr, new_arr, gaussian3, 16, output_img3);
	movefilter5x5(img_darr, new_arr, gaussian5, 256, output_img4);
	movefilter3x3(img_darr, new_arr, robert45, 1, output_img5);
	movefilter3x3(img_darr, new_arr, robert045, 1, output_img6);

	input_file = fopen(output_img5, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_img6, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//robert combine
		}
	}
	output_file = fopen(output_img7, "wb");
	fwrite(new_arr, 1, size, output_file);


	movefilter3x3(img_darr, new_arr, prewitt0, 1, output_img8);
	movefilter3x3(img_darr, new_arr, prewitt90, 1, output_img9);
	input_file = fopen(output_img8, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_img9, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//prewit combine
		}
	}
	output_file = fopen(output_img10, "wb");
	fwrite(new_arr, 1, size, output_file);

	movefilter3x3(img_darr, new_arr, sobel0, 1, output_img11);
	movefilter3x3(img_darr, new_arr, sobel90, 1, output_img12);

	input_file = fopen(output_img11, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_img12, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//sobel combine
		}
	}
	output_file = fopen(output_img13, "wb");
	fwrite(new_arr, 1, size, output_file);

	movefilter3x3(img_darr, new_arr, sobel045, 1, output_img14);
	movefilter3x3(img_darr, new_arr, sobel45, 1, output_img15);
	movefilter3x3(img_darr, new_arr, laplacian4, 1, output_img16);
	movefilter3x3(img_darr, new_arr, laplacian8, 1, output_img17);
	movefilter3x3(img_darr, new_arr, highboost_A15, 1, output_img18);
	movefilter3x3(img_darr, new_arr, highboost_A3, 1, output_img19);

	/*----------------------------with noise-----------------------------------*/

	input_file = fopen(input_img2, "rb");
	fread(img, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			img_darr[i][j] = img[i * W + j];
		}
	}
	movefilter3x3(img_darr, new_arr, box3, 9, output_noise_img1);
	movefilter5x5(img_darr, new_arr, box5, 25, output_noise_img2);
	movefilter3x3(img_darr, new_arr, gaussian3, 16, output_noise_img3);
	movefilter5x5(img_darr, new_arr, gaussian5, 256, output_noise_img4);
	movefilter3x3(img_darr, new_arr, robert45, 1, output_noise_img5);
	movefilter3x3(img_darr, new_arr, robert045, 1, output_noise_img6);
	input_file = fopen(output_noise_img5, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_noise_img6, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//robert combine
		}
	}
	output_file = fopen(output_noise_img7, "wb");
	fwrite(new_arr, 1, size, output_file);

	movefilter3x3(img_darr, new_arr, prewitt0, 1, output_noise_img8);
	movefilter3x3(img_darr, new_arr, prewitt90, 1, output_noise_img9);

	input_file = fopen(output_noise_img8, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_noise_img9, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//robert combine
		}
	}
	output_file = fopen(output_noise_img10, "wb");
	fwrite(new_arr, 1, size, output_file);

	movefilter3x3(img_darr, new_arr, sobel0, 1, output_noise_img11);
	movefilter3x3(img_darr, new_arr, sobel90, 1, output_noise_img12);
	input_file = fopen(output_noise_img11, "rb");
	fread(img_A, 1, size, input_file);
	input_file = fopen(output_noise_img12, "rb");
	fread(img_B, 1, size, input_file);
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			new_arr[i * W + j] = abs(img_A[i * W + j]) + abs(img_B[i * W + j]);	//robert combine
		}
	}
	output_file = fopen(output_noise_img13, "wb");
	fwrite(new_arr, 1, size, output_file);
	movefilter3x3(img_darr, new_arr, sobel045, 1, output_noise_img14);
	movefilter3x3(img_darr, new_arr, sobel45, 1, output_noise_img15);
	movefilter3x3(img_darr, new_arr, highboost_A15, 1, output_noise_img18);
	movefilter3x3(img_darr, new_arr, highboost_A3, 1, output_noise_img19);

	for (int i = 0; i < H; i++) {
		delete[] img_darr[i];
	}
	delete[] img, new_arr, img_darr;

}
