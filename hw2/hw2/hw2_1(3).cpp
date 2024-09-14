/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<time.h>
using namespace std;

// 往左上偏移！

//雙線性插值函數
double bilinearInterpolation(double dx, double dy, double q11, double q12, double q21, double q22) {
	double fxy1 = q11 * (1.0 - dx) * (1.0 - dy);
	double fxy2 = q12 * dx * (1.0 - dy);
	double fxy3 = q21 * (1.0 - dx) * dy;
	double fxy4 = q22 * dx * dy;

	return fxy1 + fxy2 + fxy3 + fxy4;
}

// Bicubic插值函數
double cubicInterpolate(double p[4], double x) {
	return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate(double p[4][4], double x, double y) {
	double arr[4];
	arr[0] = cubicInterpolate(p[0], y);
	arr[1] = cubicInterpolate(p[1], y);
	arr[2] = cubicInterpolate(p[2], y);
	arr[3] = cubicInterpolate(p[3], y);
	return cubicInterpolate(arr, x);
}

int _13main()
{
	char  input_img[] = "lena128.raw";                 // Input  raw image name
	char  input_256[] = "lena256.raw";
	char  input_512[] = "lena512.raw";

	char nn256[] = "1(3)_256bynn.raw";              // Output raw image name
	char nn512[] = "1(3)_512bynn.raw";              // Output raw image name
	char bi256[] = "1(3)_256bybilinear.raw";              // Output raw image name
	char bi512[] = "1(3)_512bybilinear.raw";              // Output raw image name
	char biCubic256[] = "1(3)_256bybiCubic.raw";              // Output raw image name
	char biCubic512[] = "1(3)_512bybiCubic.raw";              // Output raw image name

	int width = 128;
	int height = 128;
	int size = width * height;
	//double  scale = 256 / 128;

	int count = 1;
	double mse = 0.0;
	double  sum = 0.0;
	double psnr = 0.0;
	double START;
	double END;

	unsigned char* img_lena = new unsigned char[size]; // array for原始128資料
	unsigned char* origin_lena_256 = new unsigned char[256 * 256]; // array for原始256資料
	unsigned char* origin_lena_512 = new unsigned char[512 * 512]; // array for原始512資料
	unsigned char* img_256_lena = new unsigned char[256 * 256]; // save 256 arr
	unsigned char* img_512_lena = new unsigned char[512 * 512]; // save 512 arr

	int** darr_lena_256 = new int* [256];
	int** darr_lena_512 = new int* [512];

	for (int i = 0; i < 256; i++) {
		darr_lena_256[i] = new int[256];
	}
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

	input_file = fopen(input_256, "rb");
	fread(origin_lena_256, 1, 256 * 256, input_file);
	input_file = fopen(input_512, "rb");
	fread(origin_lena_512, 1, 512 * 512, input_file);

	/*---------------------------------------------------------------------------*/
	// nearest neighbor to 256
	START = clock();
	for (int i = 0; i < 256; i += 1) {
		for (int j = 0; j < 256; j += 1) {
			int originX = j / 2;
			int originY = i / 2;
			originX = max(0, min(originX, 128 - 1));
			originY = max(0, min(originY, 128 - 1));
			img_256_lena[i * 256 + j] = img_lena[originY * 128 + originX];
		}
	}


	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((img_256_lena[i] - origin_lena_256[i]), 2);
	}

	mse = sum / 256 / 256;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 256 by nearest neighbotr mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(nn256, "wb");
	fwrite(img_256_lena, 1, 256 * 256, output_file);
	END = clock();
	cout << "   1_(3) Nearest neighbor to 256 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;


	///*----------------------------------------------------------------------------*/
	// bilinear to 256
	START = clock();
	mse = 0.0;
	sum = 0.0;
	psnr = 0.0;
	for (int j = 0; j < 256; j += 1) {
		for (int i = 0; i < 256; i += 1) {
			double originX = (i + 0.5) * 128.0 / 256.0 - 0.5;
			double originY = (j + 0.5) * 128.0 / 256.0 - 0.5;
			int x1 = floor(originX);
			int y1 = floor(originY);
			int x2 = ceil(originX);
			int y2 = ceil(originY);

			//找出四個最近點座標並使之不超過邊界
			x1 = max(0, min(x1, 128 - 1));
			y1 = max(0, min(y1, 128 - 1));
			x2 = max(0, min(x2, 128 - 1));
			y2 = max(0, min(y2, 128 - 1));

			double dx = originX - x1;
			double dy = originY - y1;

			double q11 = img_lena[y1 * 128 + x1];
			double q12 = img_lena[y1 * 128 + x2];
			double q21 = img_lena[y2 * 128 + x1];
			double q22 = img_lena[y2 * 128 + x2];

			img_256_lena[j * 256 + i] = bilinearInterpolation(dx, dy, q11, q12, q21, q22);
		}
	}

	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((img_256_lena[i] - origin_lena_256[i]), 2);
	}

	mse = sum / 256 / 256;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 256 by bilinear interpolation mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(bi256, "wb");
	fwrite(img_256_lena, 1, 256 * 256, output_file);
	END = clock();
	cout << "   1_(3) Bilinear to 256 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;


	/*---------------------------------------------------------------------------*/
	// bicubic to 256
	START = clock();
	mse = 0.0;
	sum = 0.0;
	psnr = 0.0;

	for (int j = 0; j < 256; j++) {
		for (int i = 0; i < 256; i++) {
			double originX = (i + 0.5) * 128.0 / 256.0 - 0.5;
			double originY = (j + 0.5) * 128.0 / 256.0 - 0.5;
			int x0 = originX;
			int y0 = originY;
			double dx = originX - x0;
			double dy = originY - y0;

			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 128 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 128 - 1));
					values[n][m] = img_lena[yy * 128 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate(values, dx, dy);
			img_256_lena[j * 256 + i] = interpolatedValue;
		}
	}

	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((img_256_lena[i] - origin_lena_256[i]), 2);
	}

	mse = sum / 256 / 256;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 256 by bicubic interpolation  mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(biCubic256, "wb");
	fwrite(img_256_lena, 1, 256 * 256, output_file);
	END = clock();
	cout << "   1_(3) BiCubic to 256 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;
	cout << endl;
	
	/*---------------------------------------------------------------------------*/
	//nearest neighbor to 512
	START = clock();
	mse = 0.0;
	sum = 0.0;
	psnr = 0.0;

	for (int j = 0; j < 512; j += 1) {
		for (int i = 0; i < 512; i += 1) {
			int originX = i / 4;
			int originY = j / 4;
			originX = max(0, min(originX, 128 - 1));
			originY = max(0, min(originY, 128 - 1));
			img_512_lena[j * 512 + i] = img_lena[originY * 128 + originX];
		}
	}

	for (int i = 0; i < 512 * 512; i++) {
		sum += pow((img_512_lena[i] - origin_lena_512[i]), 2);
	}

	mse = sum / 512 / 512;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 512 by nearest neighbotr mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(nn512, "wb");
	fwrite(img_512_lena, 1, 512 * 512, output_file);
	END = clock();
	cout << "   1_(3) Nearest Neighbor to 512 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;
	///*----------------------------------------------------------------------------*/
	//// bilinear to 512
	START = clock();
	mse = 0.0;
	sum = 0.0;
	psnr = 0.0;

	for (int j = 0; j < 512; j += 1) {
		for (int i = 0; i < 512; i += 1) {
			double originX = (i + 0.5) * 128.0 / 512.0 - 0.5;
			double originY = (j + 0.5) * 128.0 / 512.0 - 0.5;
			int x1 = floor(originX);
			int y1 = floor(originY);
			int x2 = ceil(originX);
			int y2 = ceil(originY);

			//找出四個最近點座標並使之不超過邊界
			x1 = max(0, min(x1, 128 - 1));
			y1 = max(0, min(y1, 128 - 1));
			x2 = max(0, min(x2, 128 - 1));
			y2 = max(0, min(y2, 128 - 1));

			double dx = originX - x1;
			double dy = originY - y1;

			double q11 = img_lena[y1 * 128 + x1];
			double q12 = img_lena[y1 * 128 + x2];
			double q21 = img_lena[y2 * 128 + x1];
			double q22 = img_lena[y2 * 128 + x2];

			img_512_lena[j * 512 + i] = bilinearInterpolation(dx, dy, q11, q12, q21, q22);
		}
	}

	for (int i = 0; i < 512 * 512; i++) {
		sum += pow((img_512_lena[i] - origin_lena_512[i]), 2);
	}

	mse = sum / 512 / 512;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 512 by bilinear interpolation mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(bi512, "wb");
	fwrite(img_512_lena, 1, 512 * 512, output_file);
	END = clock();
	cout << "   1_(3) Bilinear to 512 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	/*---------------------------------------------------------------------------*/
	//bicubic to 512
	START = clock();
	mse = 0.0;
	sum = 0.0;
	psnr = 0.0;

	for (int j = 0; j < 512; j++) {
		for (int i = 0; i < 512; i++) {
			/*double originX = i / 4.0;
			double originY = j / 4.0;*/
			double originX = (i + 0.5) * 128.0 / 512.0 - 0.5;
			double originY = (j + 0.5) * 128.0 / 512.0 - 0.5;
			int x0 = originX;
			int y0 = originY;
			double dx = originX - x0;
			double dy = originY - y0;

			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 128 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 128 - 1));
					values[n][m] = img_lena[yy * 128 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate(values, dx, dy);
			img_512_lena[j * 512 + i] = interpolatedValue;
		}
	}

	for (int i = 0; i < 512 * 512; i++) {
		sum += pow((img_512_lena[i] - origin_lena_512[i]), 2);
	}

	mse = sum / 512 / 512;
	psnr = 20 * log10(255 / sqrt(mse));
	cout << count << ": 1_(3) 512 by bicubic interpolation  mse = " << mse << ";    psnr = " << psnr << endl;
	count++;

	output_file = fopen(biCubic512, "wb");
	fwrite(img_512_lena, 1, 512 * 512, output_file);
	END = clock();
	cout << "   1_(3) BiCubic to 512 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;


	delete[] img_lena, img_256_lena, img_512_lena;

	for (int i = 0; i < 256; i++) {
		delete[] darr_lena_256[i];
	}
	for (int i = 0; i < 512; i++) {
		delete[] darr_lena_512[i];
	}
	delete[] darr_lena_256, darr_lena_512;
	fclose(input_file);
	fclose(output_file);
}
