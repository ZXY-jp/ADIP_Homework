/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <cmath>
#include <iostream>
#include<time.h>
#include <opencv2/opencv.hpp>

#define W 256
#define H 256
#define size 256*256
#define PI 3.14159

using namespace std;
using namespace cv;

// 定義Shift 函數


double** dct(double** data, double** dctResult) {
	double cm, cv, sum;
	double temp[H][W];

	//第一次，水平
	for (int m = 0; m < W; m++) {
		for (int l = 0; l < W; l++) {
			cm = l == 0 ? sqrt(1.0 / W) : sqrt(2.0 / W);
			sum = 0;
			for (int i = 0; i < W; i++) {
				sum += data[m][i] * cos((2 * i + 1) * l * PI / (2 * W));
			}
			temp[m][l] = cm * sum;
		}
	}
	//第二次，垂直
	for (int l = 0; l < H; l++) {
		for (int k = 0; k < W; k++) {
			cv = k == 0 ? sqrt(1.0 / W) : sqrt(2.0 / W);
			sum = 0;
			for (int m = 0; m < W; m++) {
				sum += temp[m][l] * cos((2 * m + 1) * k * PI / (2 * W));
			}

			dctResult[k][l] = cv * sum;
		}
	}

	return dctResult;
}

double** idct(double** data, double** idctResult) {
	double cu, cv, sum;
	double temp[H][W];
	for (int x = 0; x < W; x++) {
		for (int y = 0; y < W; y++) {
			sum = 0;

			for (int i = 0; i < W; i++) {
				for (int j = 0; j < W; j++) {
					cu = (i == 0) ? sqrt(1.0 / W) : sqrt(2.0 / W);
					cv = (j == 0) ? sqrt(1.0 / W) : sqrt(2.0 / W);

					sum += cu * cv * data[i][j] * cos((2 * x + 1) * i * PI / (2.0 * W)) * cos((2 * y + 1) * j * PI / (2.0 * W));
				}
			}

			idctResult[x][y] = sum;
		}
	}
	return idctResult;
}

void displayImage(double* data, const char* windowName, string fileName) {
	Mat image(H, W, CV_64F);
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			image.at<double>(i, j) = data[i * W + j];
		}
	}

	image.convertTo(image, CV_8U);
	imshow(windowName, image);
	waitKey(0);
	imwrite(fileName, image);
}

void _compare(uchar* original, double* inverse, int flag) {
	int sum = 0;
	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((original[i] - inverse[i]), 2);
	}

	double mse = sum / 256 / 256;
	double psnr = 20 * log10(255 / sqrt(mse));
	if (flag == 1) {
		cout << "1_(6) Original Baboon and IDCT Baboons's mse = " << mse << ";    psnr = " << psnr << endl;
	}
	else if (flag == 2) {
		cout << "1_(6) Original Lena and IDCT Lena's mse = " << mse << ";    psnr = " << psnr << endl;
	}
}

int _156main()
{
	char  input1[] = "baboon_256.raw";                 // Input  raw image name
	char  input2[] = "lena_256.raw";                 // Input  raw image name
	double START;
	double END;
	FILE* input_file;
	FILE* output_file;
	string fileName, fileName2;
	unsigned char* baboon = new unsigned char[size]; // array for image data
	unsigned char* lena = new unsigned char[size]; // array for image data
	double* result = new double[size];
	double* _inverse = new double[size];

	double** baboon_darr = new double* [H];
	double** lena_darr = new double* [H];
	double** result_darr = new double* [H];
	double** inverse_darr = new double* [H];

	for (int i = 0; i < H; i++) {
		baboon_darr[i] = new double[W];
		lena_darr[i] = new double[W];
		result_darr[i] = new double[W];
		inverse_darr[i] = new double[W];
	}

	input_file = fopen(input1, "rb");
	fread(baboon, 1, size, input_file);
	input_file = fopen(input2, "rb");
	fread(lena, 1, size, input_file);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			baboon_darr[i][j] = baboon[i * W + j];
			lena_darr[i][j] = lena[i * W + j];
		}
	}

	/*---------------------Baboon DCT-------------------------*/
	START = clock();

	result_darr = dct(baboon_darr, result_darr);

	fileName = "1(5)baboon_DCT.png";

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			result[i * W + j] = result_darr[i][j];
		}
	}

	END = clock();
	cout << "1_(5)Baboon_DCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	displayImage(result, "DCT Magnitude Spectrum", fileName);

	/*--------------------------Baboon IDCT-----------------------------*/
	cout << endl;
	fileName2 = "1(6)baboon_IDCT.png";
	START = clock();

	inverse_darr = idct(result_darr, inverse_darr);

	//為了後續compare
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			_inverse[i * W + j] = inverse_darr[i][j];
		}
	}
	Mat image(H, W, CV_64F);
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			image.at<double>(i, j) = inverse_darr[i][j];
		}
	}
	END = clock();
	cout << "1_(6)Baboon_IDCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	displayImage(_inverse, "ReturnImage", fileName2);
	_compare(baboon, _inverse, 1);

	/*---------------------Lena DCT-------------------------*/
	cout << "------------------------" << endl;

	START = clock();

	result_darr = dct(lena_darr, result_darr);

	fileName = "1(5)lena_DCT.png";

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			result[i * W + j] = result_darr[i][j];
		}
	}
	END = clock();
	cout << "1_(5)lena_DCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	displayImage(result, "DCT Magnitude Spectrum", fileName);
	/*--------------------------Lena IDCT-----------------------------*/
	cout << endl;
	fileName2 = "1(6)lena_IDCT.png";
	START = clock();

	inverse_darr = idct(result_darr, inverse_darr);
	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			_inverse[i * W + j] = inverse_darr[i][j];
		}
	}

	for (int i = 0; i < H; ++i) {
		for (int j = 0; j < W; ++j) {
			image.at<double>(i, j) = _inverse[i * W + j];
		}
	}
	END = clock();
	cout << "1_(6)Lena_IDCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	displayImage(_inverse, "ReturnImage", fileName2);
	_compare(lena, _inverse, 2);

	/*---------------------------------------------------------------------------*/
	for (int i = 0; i < H; i++) {
		delete baboon_darr[i];
		delete lena_darr[i];
		delete result_darr[i];
		delete inverse_darr[i];
	}

	delete[] baboon, lena, baboon_darr, lena_darr, result_darr, inverse_darr;

	return 0;
}
