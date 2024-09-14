/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<time.h>
using namespace std;

//雙線性插值函數
double bilinear_14(double dx, double dy, double q11, double q12, double q21, double q22) {
	double fxy1 = q11 * dx * dy;
	double fxy2 = q12 * dx * (1.0 - dy);
	double fxy3 = q21 * (1.0 - dx) * dy;
	double fxy4 = q22 * (1.0 - dx) * (1.0 - dy);

	return fxy1 + fxy2 + fxy3 + fxy4;
}
////Bicubic插值函數
double cubicInterpolate_14(double p[4], double x) {
	return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate_14(double p[4][4], double x, double y) {
	double arr[4];
	arr[0] = cubicInterpolate_14(p[0], y);
	arr[1] = cubicInterpolate_14(p[1], y);
	arr[2] = cubicInterpolate_14(p[2], y);
	arr[3] = cubicInterpolate_14(p[3], y);
	return   cubicInterpolate_14(arr, x);
}


int _14main()
{
	char  input_img[] = "lena512_xnview_blur.raw";                 // Input  raw image name

	char nn384[] = "1(4)_384bynn.raw";              // Output raw image name
	char bi384[] = "1(4)_384bybilinear.raw";              // Output raw image name
	char bicu384[] = "1(4)_384bybicubic.raw";              // Output raw image name

	int width = 512;
	int height = 512;
	int size = width * height;
	double scale = 384 / 512;
	double START;
	double END;

	unsigned char* img_lena = new unsigned char[size]; // array for原始512資料
	unsigned char* img_384_lena = new unsigned char[384 * 384]; // save 384s arr

	int** darr_lena_384 = new int* [384];
	for (int i = 0; i < 384; i++) {
		darr_lena_384[i] = new int[384];
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

	/*---------------------------------------------------------------------------*/
	// nearest neighbor to 384
	START = clock();
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			int originX = j / 0.75;
			int originY = i / 0.75;
			originX = max(0, min(originX, 512 - 1));
			originY = max(0, min(originY, 512 - 1));
			img_384_lena[i * 384 + j] = img_lena[originY * 512 + originX];
		}
	}
	output_file = fopen(nn384, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	cout << "Nearest neighbor to 384 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	/*----------------------------------------------------------------------------*/
	// bilinear to 384
	START = clock();
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			double originX = i / 0.75;
			double originY = j / 0.75;
			int x1 = floor(originX);
			int y1 = floor(originY);
			int x2 = ceil(originX);
			int y2 = ceil(originY);

			//找出四個最近點座標並使之不超過邊界
			x1 = max(0, min(x1, 512 - 1));
			y1 = max(0, min(y1, 512 - 1));
			x2 = max(0, min(x2, 512 - 1));
			y2 = max(0, min(y2, 512 - 1));

			double dx = originX - x1;
			double dy = originY - y1;

			double q11 = img_lena[y1 * 512 + x1];
			double q12 = img_lena[y2 * 512 + x1];
			double q21 = img_lena[y1 * 512 + x2];
			double q22 = img_lena[y2 * 512 + x2];

			img_384_lena[j * 384 + i] = char(bilinear_14(dx, dy, q11, q12, q21, q22));
		}
	}

	output_file = fopen(bi384, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	cout << "Bilinear to 384 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;


	/*---------------------------------------------------------------------------*/
	// bicubic
	START = clock();
	for (int j = 0; j < 384; j++) {
		for (int i = 0; i < 384; i++) {
			double originX = i / 0.75;
			double originY = j / 0.75;
			int x0 = originX;
			int y0 = originY;
			double dx = originX - x0;
			double dy = originY - y0;

			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 512 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 512 - 1));
					values[n][m] = img_lena[yy * 512 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_14(values, dx, dy);
			img_384_lena[j * 384 + i] = interpolatedValue;
		}
	}
	output_file = fopen(bicu384, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	cout << "Bicubic to 384 所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	/*---------------------------------------------------------------------------*/

	delete[] img_lena, img_384_lena;
	fclose(input_file);
	fclose(output_file);
}
