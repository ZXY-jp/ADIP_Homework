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
};

void dft2D(unsigned char* img, ComplexNum** dst, string fileName) {
	////2D-DFT
	Mat out(H, W, CV_64F);

	for (int u = 0; u < H; u++)
	{
		for (int v = 0; v < W; v++) {
			dst[u][v].real = 0.0;
			dst[u][v].imagin = 0.0;

			for (int i = 0; i < H; i++) {
				for (int j = 0; j < W; j++) {
					double x = PI * 2.0 * ((i * u) / W + (j * v) / H);
					dst[u][v].real += cos(x) * img[i * W + j] * pow(-1, i + j);
					dst[u][v].imagin -= sin(x) * img[i * W + j] * pow(-1, i + j);
				}
			}
			/*dst[u][v].real /= double(size);
			dst[u][v].imagin /= double(size);*/
			double g = sqrt(dst[u][v].real * dst[u][v].real + dst[u][v].imagin * dst[u][v].imagin);
			cout << g << " , after log: ";
			g = log(g + 1) * 255 / log(256);
			cout << g << endl;
			out.at<double>(u, v) = g;
		}
	}
	out.convertTo(out, CV_8U);
	imshow("Magnitude of Baboon", out);
	waitKey();
	imwrite(fileName, out);

}


//void ComplextoMat(ComplexNum** dft, string fileName) {
//	Mat out(H, W, CV_64F);
//
//	for (int u = 0; u < H; u++) {
//		for (int v = 0; v < W; v++) {
//			double R = dft[u][v].real / size;
//			double I = dft[u][v].imagin / size;
//			double g = sqrt(R * R + I * I);
//			cout << g << " , after log: ";
//			g = log(g + 1) * 255 / log(256);
//			cout << g << endl;
//			out.at<double>(u, v) = g;
//		}
//	}
//	out.convertTo(out, CV_8U);
//	imshow("Magnitude of Baboon", out);
//	waitKey();
//	imwrite(fileName, out);
//}

//void displayImage_(double* data, const char* windowName, string fileName) {
//	Mat image(H, W, CV_64F);
//	for (int i = 0; i < H; ++i) {
//		for (int j = 0; j < W; ++j) {
//			image.at<double>(i, j) = data[i * W + j];
//		}
//	}
//
//	image.convertTo(image, CV_8U);
//	imshow(windowName, image);
//	waitKey(0);
//	imwrite(fileName, image);
//}

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

int _1main() {
	char  input1[] = "baboon_256.raw";                 // Input  raw image name
	char  input2[] = "lena_256.raw";                 // Input  raw image name
	double START;
	double END;
	FILE* input_file;

	string fileName, fileName2;
	unsigned char* baboon = new unsigned char[size]; // array for image data
	unsigned char* lena = new unsigned char[size]; // array for image data
	double* result = new double[size];
	//double* _inverse = new double[size];

	double** baboon_darr = new double* [H];
	double** lena_darr = new double* [H];
	double** result_darr = new double* [H];
	//double** inverse_darr = new double* [H];

	for (int i = 0; i < H; i++) {
		baboon_darr[i] = new double[W];
		//lena_darr[i] = new double[W];
		result_darr[i] = new double[W];
		//inverse_darr[i] = new double[W];
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

	/*---------------------Baboon DFT-------------------------*/
	START = clock();
	fileName = "1(1)Baboon_DFT.png";

	//ComplexNum dst[H][W] = {};
	ComplexNum** dst = new ComplexNum * [H];
	for (int i = 0; i < W; i++) {
		dst[i] = new ComplexNum[W];
	}

	dft2D(baboon, dst, fileName);
	//ComplextoMat(dst, fileName);

	END = clock();
	cout << "1_(1)Baboon_DCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	//displayImage_(result, "DCT Magnitude Spectrum", fileName);

	/*--------------------------Baboon IDCT-----------------------------*/
	//cout << endl;
	//fileName2 = "1(6)baboon_IDCT.png";
	//START = clock();

	//inverse_darr = idft(result_darr, inverse_darr);

	////為了後續compare
	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		_inverse[i * W + j] = inverse_darr[i][j];
	//	}
	//}
	//Mat image(H, W, CV_64F);
	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		image.at<double>(i, j) = inverse_darr[i][j];
	//	}
	//}
	//END = clock();
	//cout << "1_(6)Baboon_IDCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

	//displayImage_(_inverse, "ReturnImage", fileName2);
	//compare_(baboon, _inverse, 1);

	///*---------------------Lena DCT-------------------------*/
	//cout << "------------------------" << endl;

	//START = clock();

	//result_darr = dft(lena_darr, result_darr);

	//fileName = "1(5)lena_DCT.png";

	//for (int i = 0; i < H; ++i) {
	//	for (int j = 0; j < W; ++j) {
	//		result[i * W + j] = result_darr[i][j];
	//	}
	//}
	//END = clock();
	//cout << "1_(5)lena_DCT所費時間: " << (END - START) / CLOCKS_PER_SEC << endl;

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

