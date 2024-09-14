/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include <time.h> 
#include<cmath>
using namespace std;

// 雙線性插值函數
double bilinear(double dx, double dy, double q11, double q12, double q21, double q22) {
	double fxy1 = q11 * (1.0 - dx) * (1.0 - dy);
	double fxy2 = q12 * dx * (1.0 - dy);
	double fxy3 = q21 * (1.0 - dx) * dy;
	double fxy4 = q22 * dx * dy;

	return fxy1 + fxy2 + fxy3 + fxy4;
}

// Bicubic插值函數
double cubicInterpolate_(double p[4], double x) {
	return p[1] + 0.5 * x * (p[2] - p[0] + x * (2.0 * p[0] - 5.0 * p[1] + 4.0 * p[2] - p[3] + x * (3.0 * (p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate_(double p[4][4], double x, double y) {
	double arr[4];
	arr[0] = cubicInterpolate_(p[0], y);
	arr[1] = cubicInterpolate_(p[1], y);
	arr[2] = cubicInterpolate_(p[2], y);
	arr[3] = cubicInterpolate_(p[3], y);
	return cubicInterpolate_(arr, x);
}


int _15main()
{
	char input_img[] = "lena256.raw";                 // Input  raw image name
	char nn1[] = "1(5)_bynn1.raw";              // Output raw image name
	char nn2[] = "1(5)_bynn2.raw";              // Output raw image name
	char nn3[] = "1(5)_bynn3.raw";              // Output raw image name
	char bi1[] = "1(5)_bybilinear1.raw";              // Output raw image name
	char bi2[] = "1(5)_bybilinear2.raw";              // Output raw image name
	char bi3[] = "1(5)_bybilinear3.raw";              // Output raw image name
	char bicu1[] = "1(5)_bybicubic1.raw";              // Output raw image name
	char bicu2[] = "1(5)_bybicubic2.raw";              // Output raw image name
	char bicu3[] = "1(5)_bybicubic3.raw";              // Output raw image name
	int counter = 0;
	int width = 256;
	int height = 256;
	int size = width * height;
	unsigned char* img_lena = new unsigned char[size]; // array for原始資料
	unsigned char* img_576_lena = new unsigned char[576 * 576]; // save new arr
	unsigned char* img_384_lena = new unsigned char[384 * 384]; // save new arr
	unsigned char* img_new_lena = new unsigned char[300 * 300]; // save new arr
	double* timer = new  double[10]; // array for原始資料

	double START, END;
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
	// nearest neighbor1 先放大再縮小
	START = clock();
	double scale = 2.25;
	for (int i = 0; i < 576; i += 1) {
		for (int j = 0; j < 576; j += 1) {
			int originX = j / scale;
			int originY = i / scale;
			img_576_lena[i * 576 + j] = img_lena[originY * 256 + originX];
		}
	}
	scale = 1 / 1.5;
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			int originX = j / scale;
			int originY = i / scale;
			img_384_lena[i * 384 + j] = img_576_lena[originY * 576 + originX];
		}
	}

	output_file = fopen(nn1, "wb");
	if (output_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	fclose(output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;
	/*---------------------------------------------------------------------------*/
	// nearest neighbor2 先縮小再放大
	START = clock();
	scale = 1 / 1.5;
	for (int i = 0; i < 256 * scale; i += 1) {
		for (int j = 0; j < 256 * scale; j += 1) {
			int originX = j / scale;
			int originY = i / scale;
			originX = max(0, min(originX, 256 - 1));
			originY = max(0, min(originY, 256 - 1));
			img_new_lena[i * 170 + j] = img_lena[originY * 256 + originX];
		}
	}
	scale = 2.25;
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			int originX = j / scale;
			int originY = i / scale;
			originX = max(0, min(originX, 170 - 1));
			originY = max(0, min(originY, 170 - 1));

			img_384_lena[i * 384 + j] = img_new_lena[originY * 170 + originX];
		}
	}
	output_file = fopen(nn2, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	fclose(output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// nearest neighbor3 一步到位
	START = clock();
	scale = 1.5;
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			int originX = j / scale;
			int originY = i / scale;
			img_384_lena[i * 384 + j] = img_lena[originY * 256 + originX];
		}
	}

	output_file = fopen(nn3, "wb");
	if (output_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	fclose(output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;


	/*----------------------------------------------------------------------------*/
	// bilinear to 384 +2.25-1.5
	START = clock();
	scale = 2.25;
	for (int i = 0; i < height * scale; i += 1) {
		for (int j = 0; j < width * scale; j += 1) {
			double originX = i / scale;
			double originY = j / scale;
			int x1 = originX;
			int y1 = originY;
			int x2 = x1 + 1;
			int y2 = y1 + 1;

			int dx = originX - x1;
			int dy = originY - y1;

			int q11 = img_lena[y1 * width + x1];
			int q12 = img_lena[y1 * width + x2];
			int q21 = img_lena[y2 * width + x1];
			int q22 = img_lena[y2 * width + x2];

			img_576_lena[j * 576 + i] = bilinear(dx, dy, q11, q12, q21, q22);
		}
	}

	scale = 1 / 1.5;
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			int originX = i / scale;
			int originY = j / scale;
			int x1 = originX;
			int y1 = originY;
			int x2 = x1 + 1;
			int y2 = y1 + 1;

			int dx = originX - x1;
			int dy = originY - y1;

			int q11 = img_576_lena[y1 * 576 + x1];
			int q12 = img_576_lena[y1 * 576 + x2];
			int q21 = img_576_lena[y2 * 576 + x1];
			int q22 = img_576_lena[y2 * 576 + x2];

			img_384_lena[j * 384 + i] = bilinear(dx, dy, q11, q12, q21, q22);
		}
	}
	output_file = fopen(bi1, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// bilinear to 384 -1.5+2.5
	START = clock();
	scale = 1 / 1.5;
	for (int i = 0; i < 170; i += 1) {
		for (int j = 0; j < 170; j += 1) {
			double originX = i / scale;
			double originY = j / scale;
			int x1 = originX;
			int y1 = originY;
			int x = max(0, min(x1, 256 - 1));
			int y = max(0, min(y1, 256 - 1));
			int x2 = x + 1;
			int y2 = y + 1;

			int dx = originX - x1;
			int dy = originY - y1;

			int q11 = img_lena[y * width + x];
			int q12 = img_lena[y * width + x2];
			int q21 = img_lena[y2 * width + x];
			int q22 = img_lena[y2 * width + x2];

			img_new_lena[j * 170 + i] = bilinear(dx, dy, q11, q12, q21, q22);
		}
	}

	scale = 2.25;
	for (int i = 0; i < 384; i += 1) {
		for (int j = 0; j < 384; j += 1) {
			double originX = i / scale;
			double originY = j / scale;
			int x1 = originX;
			int y1 = originY;
			int x = max(0, min(x1, 170 - 1));
			int y = max(0, min(y1, 170 - 1));
			int x2 = x + 1;
			int y2 = y + 1;

			int dx = originX - x1;
			int dy = originY - y1;

			int q11 = img_new_lena[y * 170 + x];
			int q12 = img_new_lena[y * 170 + x2];
			int q21 = img_new_lena[y2 * 170 + x];
			int q22 = img_new_lena[y2 * 170 + x2];

			img_384_lena[j * 384 + i] = bilinear(dx, dy, q11, q12, q21, q22);
		}
	}
	output_file = fopen(bi2, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// bilinear to 384 +1.5
	START = clock();
	scale = 1.5;
	for (int i = 0; i < height * scale; i += 1) {
		for (int j = 0; j < width * scale; j += 1) {
			double originX = i / scale;
			double originY = j / scale;
			int x1 = originX;
			int y1 = originY;

			int x = max(0, min(x1, 256 - 1));
			int y = max(0, min(y1, 256 - 1));
			int x2 = x + 1;
			int y2 = y + 1;

			int dx = originX - x1;
			int dy = originY - y1;

			double q11 = img_lena[y * width + x];
			double q12 = img_lena[y * width + x2];
			double q21 = img_lena[y2 * width + x];
			double q22 = img_lena[y2 * width + x2];

			img_384_lena[j * 384 + i] = bilinear(dx, dy, q11, q12, q21, q22);
		}
	}

	output_file = fopen(bi3, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// bicubic +2.25-1.5
	START = clock();
	scale = 2.25;
	for (int j = 0; j < 576; j++) {
		for (int i = 0; i < 576; i++) {
			double originX = i / scale;
			double originY = j / scale;
			int x0 = originX;
			int y0 = originY;
			double dx = originX - x0;
			double dy = originY - y0;


			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 256 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 256 - 1));
					values[n][m] = img_lena[yy * 256 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_(values, dx, dy);
			img_576_lena[j * 576 + i] = interpolatedValue;
		}
	}

	scale = 1 / 1.5;
	for (int j = 0; j < 384; j++) {
		for (int i = 0; i < 384; i++) {
			double originX = i / scale;
			double originY = j / scale;
			int x0 = originX;
			int y0 = originY;
			float dx = originX - x0;
			float dy = originY - y0;


			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 576 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 576 - 1));
					values[n][m] = img_576_lena[yy * 576 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_(values, dx, dy);
			img_384_lena[j * 384 + i] = interpolatedValue;
		}
	}
	output_file = fopen(bicu1, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// bicubic -1.5+2.25
	START = clock();
	scale = 1 / 1.5;
	for (int j = 0; j < height * scale; j++) {
		for (int i = 0; i < width * scale; i++) {
			double originX = i / scale;
			double originY = j / scale;
			int x0 = originX;
			int y0 = originY;
			float dx = originX - x0;
			float dy = originY - y0;


			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 256 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 256 - 1));
					values[n][m] = img_lena[yy * 256 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_(values, dx, dy);
			img_new_lena[j * 170 + i] = interpolatedValue;
		}
	}

	scale = 2.25;
	for (int j = 0; j < 384; j++) {
		for (int i = 0; i < 384; i++) {
			double originX = i / scale;
			double originY = j / scale;
			int x0 = originX;
			int y0 = originY;
			float dx = originX - x0;
			float dy = originY - y0;


			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 170 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 170 - 1));
					values[n][m] = img_new_lena[yy * 170 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_(values, dx, dy);
			img_384_lena[j * 384 + i] = interpolatedValue;
		}
	}
	output_file = fopen(bicu2, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	// bicubic +1.5
	START = clock();
	scale = 1.5;
	for (int j = 0; j < 384; j++) {
		for (int i = 0; i < 384; i++) {
			double originX = i / scale;
			double originY = j / scale;
			int x0 = originX;
			int y0 = originY;
			float dx = originX - x0;
			float dy = originY - y0;


			double values[4][4];
			//取得鄰近16格
			for (int m = 0; m < 4; m++) {
				int yy = max(0, min(y0 + m - 1, 256 - 1));
				for (int n = 0; n < 4; n++) {
					int xx = max(0, min(x0 + n - 1, 256 - 1));
					values[n][m] = img_lena[yy * 256 + xx];
				}
			}
			double interpolatedValue = bicubicInterpolate_(values, dx, dy);
			img_384_lena[j * 384 + i] = interpolatedValue;

		}
	}
	output_file = fopen(bicu3, "wb");
	fwrite(img_384_lena, 1, 384 * 384, output_file);
	END = clock();
	timer[counter] = (END - START) / CLOCKS_PER_SEC;
	counter++;

	/*---------------------------------------------------------------------------*/
	cout << "Nearest neighbor先放大2.25再縮小1.5所花費的時間：" << timer[0] << " s" << endl;
	cout << "Bilinear先放大2.25再縮小1.5所花費的時間：" << timer[3] << " s" << endl;
	cout << "BiCubic先放大2.25倍再縮小1.5倍所花費的時間：" << timer[6] << " s" << endl;
	cout << endl;
	cout << "Nearest neighbor先縮小1.5再放大2.25所花費的時間：" << timer[1] << " s" << endl;
	cout << "Bilinear先縮小1.5再放大2.25所花費的時間：" << timer[4] << " s" << endl;
	cout << "BiCubic縮小1.5倍再放大2.25倍所花費的時間：" << timer[7] << " s" << endl;
	cout << endl;
	cout << "Nearest neighbor直接放大1.5倍所花費的時間：" << timer[2] << " s" << endl;
	cout << "Bilinear放大1.5倍所花費的時間：" << timer[5] << " s" << endl;
	cout << "BiCubic放大1.5倍所花費的時間：" << timer[8] << " s" << endl;


	delete[] img_lena, img_384_lena, img_576_lena, timer;

	fclose(input_file);
	fclose(output_file);
}
