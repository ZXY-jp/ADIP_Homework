/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <math.h>
#include <iostream>
#define W 256
#define H 256
#define size 256*256
#define PI 3.14
using namespace cv;
using namespace std;

struct ComplexNum {
	double real;
	double imagin;

	ComplexNum() : real(0.0), imagin(0.0) {}
	ComplexNum(double real, double imag) : real(real), imagin(imag) {}

	//ComplexNum operator+(const ComplexNum& other) const {
	//	return { real + other.real, imagin + other.imagin };
	//}

	//ComplexNum operator*(const ComplexNum& other) const {
	//	return { real * other.real - imagin * other.imagin, real * other.imagin + imagin * other.real };
	//}
};


void idft2D(unsigned char* img, ComplexNum** dst, string fileName) {
	////2D-DFT
	Mat out(H, W, CV_64F);

	for (int u = 0; u < H; u++)
	{
		for (int v = 0; v < W; v++) {
			dst[u][v].real = 0.0;
			dst[u][v].imagin = 0.0;

			for (int i = 0; i < H; i++) {
				for (int j = 0; j < W; j++) {
					double x = PI * 2 * ((i * u) / W + (j * v) / H);
					dst[u][v].real += cos(x) * img[i * W + j] * pow(-1, i + j);
					dst[u][v].imagin -= sin(x) * img[i * W + j] * pow(-1, i + j);
				}
			}
			double R = dst[u][v].real / size;
			double I = dst[u][v].imagin / size;
			double g = sqrt(R * R + I * I);
			//cout << g << " , after log: ";
			g = log(g + 1) * 255 / log(256);
			//cout << g << endl;
			out.at<double>(u, v) = g;
		}
	}
	out.convertTo(out, CV_8U);
	imshow("Magnitude of Baboon", out);
	waitKey();
	imwrite(fileName, out);

}



//void compare_(uchar* original, double* inverse, int flag) {
//	int sum = 0;
//	for (int i = 0; i < 256 * 256; i++) {
//		sum += pow((original[i] - inverse[i]), 2);
//	}
//
//	double mse = sum / 256 / 256;
//	double psnr = 20 * log10(255 / sqrt(mse));
//	if (flag == 1) {
//		cout << "1_(6) Original Baboon and IDCT Baboons's mse = " << mse << ";    psnr = " << psnr << endl;
//	}
//	else if (flag == 2) {
//		cout << "1_(6) Original Lena and IDCT Lena's mse = " << mse << ";    psnr = " << psnr << endl;
//	}
//}


int _12main() {
	char  input1[] = "1(3)magnitude_baboon.png";                 // Input  raw image name
	char  input2[] = "1(3)magnitude_lena.png";                 // Input  raw image name
	double START;
	double END;
	FILE* input_file;

	string fileName, fileName2;

	unsigned char* baboon = new unsigned char[size]; // array for image data
	unsigned char* lena = new unsigned char[size]; // array for image data
	double* result = new double[size];


	double** baboon_darr = new double* [H];
	double** lena_darr = new double* [H];
	double** result_darr = new double* [H];

	for (int i = 0; i < H; i++) {
		baboon_darr[i] = new double[W];
		lena_darr[i] = new double[W];
		result_darr[i] = new double[W];
	}

	input_file = fopen(input1, "rb");
	fread(baboon, 1, size, input_file);
	//input_file = fopen(input2, "rb");
	//fread(lena, 1, size, input_file);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			baboon_darr[i][j] = baboon[i * W + j];
			//lena_darr[i][j] = lena[i * W + j];
		}
	}

	/*---------------------Baboon IDFT-------------------------*/
	START = clock();
	fileName = "1(2)Baboon_myIDFT.png";
	
	ComplexNum** dst = new ComplexNum * [H];
	for (int i = 0; i < W; i++) {
		dst[i] = new ComplexNum[W];
	}

	idft2D(baboon, dst, fileName);
	//ComplextoMat(dst, fileName);

	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		result[i * W + j] = result_darr[i][j];
	//	}
	//}

	END = clock();
	cout << "1_(1)Baboon_DCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	//displayImage_(result, "DCT Magnitude Spectrum", fileName);

	///*--------------------------Lena IDCT-----------------------------*/
	//cout << endl;
	//fileName2 = "1(6)lena_IDCT.png";
	//START = clock();

	//inverse_darr = idft(result_darr, inverse_darr);
	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		_inverse[i * W + j] = inverse_darr[i][j];
	//	}
	//}

	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		image.at<double>(i, j) = _inverse[i * W + j];
	//	}
	//}
	//END = clock();
	//cout << "1_(6)Lena_IDCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	//displayImage_(_inverse, "ReturnImage", fileName2);
	//compare_(lena, _inverse, 2);

	/*---------------------------------------------------------------------------*/
	for (int i = 0; i < H; i++) {
		delete baboon_darr[i];
		delete lena_darr[i];
		delete result_darr[i];
	}

	delete[] baboon, lena, baboon_darr, lena_darr, result_darr;

	return 0;
}

