/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <string>
#define width 256
#define height 256
#define size  65536
using namespace std;

double A[8][8] = { 0 };
double c[8];

struct Point {
	int x;
	int y;
};


void GaussJordan() {
	//for (int i = 0; i < 8; i++) {
	//	// 將當前列的主元素歸一
	//	double divisor = A[i][i];
	//	for (int j = 0; j <= 8; j++) {
	//		A[i][j] /= divisor;
	//	}

	//	// 將其他行的對應元素變為零
	//	for (int k = 0; k < 8; k++) {
	//		if (k != i) {
	//			double factor = A[k][i];
	//			for (int j = 0; j <= 8; j++) {
	//				A[k][j] -= factor * A[i][j];
	//			}
	//			c[k] = c[k] - A[k][i] * c[k];
	//		}
	//	}
	//}
	//for (int i = 8 - 1; i >= 0; i--)
	//{
	//	int u = 0;
	//	for (int j = i + 1; j < 8; j++)
	//		u = u + A[i][j] * c[j];
	//	c[i] = (c[i] - u) / A[i][i];
	//}
	int N = 8;
	for (int i = 0; i < N; i++) {
		// 將當前列的主元素歸一
		double divisor = A[i][i];
		for (int j = 0; j < N; j++) {
			A[i][j] /= divisor;
		}
		c[i] /= divisor;

		// 將其他行的對應元素變為零
		for (int k = 0; k < N; k++) {
			if (k != i) {
				double factor = A[k][i];
				for (int j = 0; j < N; j++) {
					A[k][j] -= factor * A[i][j];
				}
				c[k] -= factor * c[i];
			}
		}
	}

	for (int k = 0; k < 8; k++) {
		cout << c[k] << " " << endl;
	}
}

void _3main()
{
	char map[] = "chessboard_256.raw";
	char map_distorted[] = "chessboard_distorted_256.raw";
	char cat_distorter[] = "cat_ distorted _256.raw";
	char cat[] = "cat_256.raw";

	unsigned char* map_arr = new unsigned char[size]; // array for image data
	unsigned char* map_distorted_arr = new unsigned char[size]; // array for image data
	unsigned char* cat_distorter_arr = new unsigned char[size]; // array for image data

	unsigned char* return_chessboard = new unsigned char[size]; // array for image data


	FILE* file;
	FILE* output_file;

	file = fopen(map, "rb");
	fread(map_arr, 1, size, file);
	file = fopen(map_distorted, "rb");
	fread(map_distorted_arr, 1, size, file);
	/*file = fopen(cat_distorter, "rb");
	fread(cat_distorter_arr, 1, size, file);*/

	double** schess_darr = new double* [width];
	double** chess_darr = new double* [width];
	double** scat_darr = new double* [width];

	for (int i = 0; i < height; i++) {
		schess_darr[i] = new double[width];
		chess_darr[i] = new double[width];
		scat_darr[i] = new double[width];
	}

	//將原始轉為2維方便filter通過
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			schess_darr[i][j] = map_arr[i * width + j];
			chess_darr[i][j] = map_distorted_arr[i * width + j];
			scat_darr[i][j] = cat_distorter_arr[i * width + j];
		}
	}

	/*---------------------------------------找到頂點--------------------------------------------*/
	Point pointA;
	Point pointB;
	Point pointC;
	Point pointD;


	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (chess_darr[i][j] != 128) {
				pointA.x = j;
				pointA.y = i;
				break;
			}
		}
	}
	cout << "pointA: " << pointA.x << ", " << pointA.y << endl;	//原本對應：左下
	for (int i = width - 1; i >= 0; i--) {
		for (int j = height - 1; j >= 0; j--) {
			if (chess_darr[j][i] != 128) {
				pointB.x = i;
				pointB.y = j;
				break;
			}
		}
	}
	cout << "pointB: " << pointB.x << ", " << pointB.y << endl;//原本對應：左上

	for (int i = height - 1; i >= 0; i--) {
		for (int j = width - 1; j >= 0; j--) {
			if (chess_darr[i][j] != 128) {
				pointC.x = j;
				pointC.y = i;
				break;
			}
		}
	}
	cout << "pointC: " << pointC.x << ", " << pointC.y << endl;//原本對應：右上

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (chess_darr[j][i] != 128) {
				pointD.x = i;
				pointD.y = j;
				break;
			}
		}
	}
	cout << "pointD: " << pointD.x << ", " << pointD.y << endl << endl;//原本對應：右下
	/*-------------------------------------------------------------------------------------------*/
	Point fixed[4];
	fixed[0].x = 0; fixed[0].y = 0;
	fixed[1].x = width - 1; fixed[1].y = 0;
	fixed[2].x = 0; fixed[2].y = height - 1;
	fixed[3].x = width - 1; fixed[3].y = height - 1;

	//原始座標A * 變換係數b = 扭曲後位置point
	for (int i = 0; i < 8; i++) {
		if (i < 4) {
			A[i][0] = fixed[i].x;
			A[i][1] = fixed[i].y;
			A[i][2] = fixed[i].x * fixed[i].y;
			A[i][3] = 1;
		}
		else {
			A[i][4] = fixed[i - 4].x;
			A[i][5] = fixed[i - 4].y;
			A[i][6] = fixed[i - 4].x * fixed[i - 4].y;
			A[i][7] = 1;
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	GaussJordan();	//取得a~h

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double double_y = c[0] * i + c[1] * j + c[2] * i * j + c[3];
			double double_x = c[4] * i + c[5] * j + c[6] * i * j + c[7];
			int y = (int)double_y;
			int x = (int)double_x;

			return_chessboard[i * width + j] = chess_darr[y][x];
			cout << "(x,y) = (" << x << ", " << y << ") " << chess_darr[y][x] << " ";
		}
		cout << endl;
	}

	output_file = fopen("3_cat_256.raw", "wb");
	fwrite(return_chessboard, 1, size, output_file);

	// Stop here and wait for any key even
	fclose(file);
	delete[] map_arr, map_distorted_arr, cat_distorter_arr;
}
