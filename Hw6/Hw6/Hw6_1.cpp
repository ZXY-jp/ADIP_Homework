/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <string>
#define width 256
#define height 256
#define size  65536

using namespace std;

void Adaptive_Local_Noise_Reduction(double** img, double variance_mu, const char* fileName, int filtersize) {
	FILE* output_file;
	unsigned char* new_arr = new unsigned char[size]; // array for image data
	int half = (filtersize - 1) / 2;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double a = 0;
			//計算filter裡面總和
			for (int dy = -half; dy <= half; dy++) {
				for (int dx = -half; dx <= half; dx++) {
					int ny = i + dy;
					int nx = j + dx;
					// 檢查邊界，避免超出圖像範圍
					if (ny < 0 || ny >= height) ny = 0;
					if (nx < 0 || nx >= width) nx = 0;
					a += img[ny][nx];
				}
			}
			double mL = a / filtersize / filtersize;
			//cout << "in Filter mean is: " << mL << endl;

			a = 0;
			for (int dy = -half; dy <= half; dy++) {
				for (int dx = -half; dx <= half; dx++) {
					int ny = i + dy;
					int nx = j + dx;
					// 檢查邊界，避免超出圖像範圍
					if (ny < 0 || ny >= height) ny = 0;
					if (nx < 0 || nx >= width) nx = 0;
					a += pow(img[ny][nx] - mL, 2);
				}
			}

			double varianceL = a / filtersize / filtersize;
			double variance = variance_mu / varianceL;
			if (variance > 1) {
				variance = 1;
			}
			//cout << "in Filter variance is: " << varianceL << endl;

			new_arr[i * width + j] = img[i][j] - variance * (img[i][j] - mL);
		}
	}

	output_file = fopen(fileName, "wb");
	fwrite(new_arr, 1, size, output_file);
}

void Alpha_Trimemed_Mean(double** img, const char* fileName, int filtersize, int d) {
	FILE* output_file;
	unsigned char* new_arr = new unsigned char[size]; // array for image data
	int half = (filtersize - 1) / 2;
	int* temp = new int[filtersize * filtersize]; // array for image data

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int t = 0;
			for (int dy = -half; dy <= half; dy++) {
				for (int dx = -half; dx <= half; dx++) {
					int ny = i + dy;
					int nx = j + dx;
					// 檢查邊界，避免超出圖像範圍
					if (ny < 0 || ny >= height) ny = 0;
					if (nx < 0 || nx >= width) nx = 0;
					temp[t] = img[ny][nx];
					t++;
				}
			}
			sort(temp, temp + filtersize * filtersize);
			int sum = 0;
			for (int k = d; k < filtersize * filtersize - d; k++) {
				sum += temp[k];
			}
			new_arr[i * width + j] = sum / (filtersize * filtersize - 2*d);
		}
	}

	output_file = fopen(fileName, "wb");
	fwrite(new_arr, 1, size, output_file);
	delete[] temp;
}


double mean_and_variance(unsigned char* img) {
	//統計原始圖像的mean、標準差
	int sum = 0;

	for (int i = 0; i < size; i++) {
		int m = img[i];	//灰階值
		sum += m;
	}
	cout << sum << endl;
	double mean = sum / 256 / 256;
	cout << "meam = " << mean << endl;

	double variance = 0;
	sum = 0;
	for (int i = 0; i < size; i++) {
		int m = img[i];	//灰階值
		sum += pow(m - mean, 2);
	}
	double variance_mu = sum / size;
	cout << "variance_mu = " << variance_mu << endl;

	return variance_mu;
}

void _1main()
{
	char img_noise[] = "lena_256_salt&pepper.raw";
	char img_1[] = "lena_gussuian_1_256.raw";
	char img_2[] = "lena_gussuian_2_256.raw";

	char A_3x3[] = "1(1)salt&pepper_3x3denoise.raw";
	char B_3x3[] = "1(1)lena1_3x3denoise.raw";
	char C_3x3[] = "1(1)lena2_3x3denoise.raw";
	char A_7x7[] = "1(1)salt&pepper_7x7denoise.raw";
	char B_7x7[] = "1(1)lena1_7x7denoise.raw";
	char C_7x7[] = "1(1)lena2_7x7denoise.raw";

	char A_3x3_d2[] = "1(2)salt&pepper_3x3_d2.raw";
	char B_3x3_d2[] = "1(2)lena1_3x3_d2.raw";
	char C_3x3_d2[] = "1(2)lena2_3x3_d2.raw";

	char A_7x7_d2[] = "1(2)salt&pepper_7x7_d2.raw";
	char B_7x7_d2[] = "1(2)lena1_7x7_d2.raw";
	char C_7x7_d2[] = "1(2)lena2_7x7_d2.raw";

	char A_3x3_d4[] = "1(2)salt&pepper_3x3_d4.raw";
	char B_3x3_d4[] = "1(2)lena1_3x3_d4.raw";
	char C_3x3_d4[] = "1(2)lena2_3x3_d4.raw";

	char A_7x7_d7[] = "1(2)salt&pepper_7x7_d7.raw";
	char B_7x7_d7[] = "1(2)lena1_7x7_d7.raw";
	char C_7x7_d7[] = "1(2)lena2_7x7_d7.raw";

	/*-------------------------------Spatial Domain--------------------------------------*/
	unsigned char* img_arr = new unsigned char[size]; // array for image data
	unsigned char* img_arr1 = new unsigned char[size]; // array for image data
	unsigned char* img_arr2 = new unsigned char[size]; // array for image data

	FILE* file;
	FILE* output_file;

	file = fopen(img_noise, "rb");
	fread(img_arr, 1, size, file);
	file = fopen(img_1, "rb");
	fread(img_arr1, 1, size, file);
	file = fopen(img_2, "rb");
	fread(img_arr2, 1, size, file);

	double** img_darr = new double* [width];
	double** img_darr1 = new double* [width];
	double** img_darr2 = new double* [width];

	for (int i = 0; i < height; i++) {
		img_darr[i] = new double[width];
		img_darr1[i] = new double[width];
		img_darr2[i] = new double[width];
	}

	//將原始轉為2維方便filter通過
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			img_darr[i][j] = img_arr[i * width + j];
			img_darr1[i][j] = img_arr1[i * width + j];
			img_darr2[i][j] = img_arr2[i * width + j];
		}
	}

	int filtersize = 3;
	int d;

	double variance_mu = mean_and_variance(img_arr);
	Adaptive_Local_Noise_Reduction(img_darr, variance_mu, A_3x3, filtersize);
	
	double variance_mu1 = mean_and_variance(img_arr1);
	//cout << "in lena1 variance is: " << variance_mu << endl;
	Adaptive_Local_Noise_Reduction(img_darr1, variance_mu, B_3x3, filtersize);
	
	double variance_mu2 = mean_and_variance(img_arr2);
	//cout << "in lena2 variance is: " << variance_mu << endl;
	Adaptive_Local_Noise_Reduction(img_darr2, variance_mu, C_3x3, filtersize);

	d =2;
	Alpha_Trimemed_Mean(img_darr, A_3x3_d2, filtersize, d);
	Alpha_Trimemed_Mean(img_darr1, B_3x3_d2, filtersize, d);
	Alpha_Trimemed_Mean(img_darr2, C_3x3_d2, filtersize, d);

	d = 4;
	Alpha_Trimemed_Mean(img_darr, A_3x3_d4, filtersize, d);
	Alpha_Trimemed_Mean(img_darr1, B_3x3_d4, filtersize, d);
	Alpha_Trimemed_Mean(img_darr2, C_3x3_d4, filtersize, d);

	/*----------------------------------------------------------------------------------*/
	filtersize = 7;
	Adaptive_Local_Noise_Reduction(img_darr, variance_mu,   A_7x7, filtersize);
	Adaptive_Local_Noise_Reduction(img_darr1, variance_mu1, B_7x7, filtersize);
	Adaptive_Local_Noise_Reduction(img_darr2, variance_mu2, C_7x7, filtersize);

	d = 2;
	Alpha_Trimemed_Mean(img_darr, A_7x7_d2, filtersize, d);
	Alpha_Trimemed_Mean(img_darr1, B_7x7_d2, filtersize, d);
	Alpha_Trimemed_Mean(img_darr2, C_7x7_d2, filtersize, d);
	d = 7;
	Alpha_Trimemed_Mean(img_darr, A_7x7_d7, filtersize, d);
	Alpha_Trimemed_Mean(img_darr1, B_7x7_d7, filtersize, d);
	Alpha_Trimemed_Mean(img_darr2, C_7x7_d7, filtersize, d);

	/*-------------------------------------------------------------------------------------------*/


	fclose(file);
	delete[] img_arr, img_arr1, img_arr2;
}