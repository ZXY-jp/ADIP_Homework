#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cmath>
#include <time.h>
#include <opencv2/opencv.hpp>
#define PI 3.14
#define width  474
#define height  632
#define _size  474*632

using namespace std;
using namespace cv;


unsigned char* movefilter3x3(int** img, unsigned char* new_arr, double filter[3][3], int avg) {
	FILE* output_file;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double a = 0;
			//計算filter裡面總和
			for (int dy = -1; dy <= 1; dy++) {
				for (int dx = -1; dx <= 1; dx++) {
					int ny = i + dy;
					int nx = j + dx;
					// 檢查邊界，避免超出圖像範圍
					if (ny < 0) ny = 0;
					if (ny >= height) ny = height - 1;
					if (nx < 0) nx = 0;
					if (nx >= width) nx = width - 1;
					a += img[ny][nx] * filter[dy + 1][dx + 1];
				}
			}
			if (a / avg > 255)
				new_arr[i * width + j] = 255;
			else if (a / avg < 0)
				new_arr[i * width + j] = 0;
			else
				new_arr[i * width + j] = a / avg;
		}
	}
	return new_arr;
}

// 定義Shift 函數
void shift_3(Mat& mag) {
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));

	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

Mat filterConvert(Mat InMat) {
	Mat complexImage;

	dft(InMat, complexImage, DFT_COMPLEX_OUTPUT);
	//_shift(filter);	//將原先移動道邊邊過的filter移動到中間
	Mat planes[2];
	split(complexImage, planes);// 分離實部和虛部

	//// 計算幅度頻譜
	Mat magnitude;
	cv::magnitude(planes[0], planes[1], magnitude);

	//// 正規化以在0到255之間顯示
	normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);	//HPF要1

	////轉存成8bit資料(0~255)
	Mat _magnitude;
	magnitude.convertTo(_magnitude, CV_8U, 255);
	//shift_3(_magnitude);	//移動到中間

	//imshow("Merge Image", _magnitude);	//加上filter的頻譜
	//waitKey(0);
	return _magnitude;
}

Mat opencv_DFT3(Mat InMat, Mat filter, string magnitudeName,string kernelName) {
	// 進行DFT
	Mat complexImage;

	Mat filterX;
	dft(InMat, complexImage, DFT_COMPLEX_OUTPUT);
	imshow("Sobel", filter);	//加上filter的頻譜
	waitKey(0);
	shift_3(filter);	//將原先移動道邊邊過的filter移動到中間

	Mat planes[2];
	split(complexImage, planes);// 分離實部和虛部

	////進行filter相乘
	cout << planes[0].size << " " << filter.size << endl;
	cout << planes[0].depth() << " " << filter.depth() << endl;
	multiply(planes[0], filter, planes[0]);
	multiply(planes[1], filter, planes[1]);
	cv::merge(planes, 2, complexImage);

	//// 計算幅度頻譜
	Mat magnitude;
	cv::magnitude(planes[0], planes[1], magnitude);

	//// 正規化以在0到255之間顯示
	normalize(magnitude, magnitude, 0, 1, NORM_MINMAX);	//HPF要1

	////轉存成8bit資料(0~255)
	Mat _magnitude;
	magnitude.convertTo(_magnitude, CV_8U, 255);
	shift_3(_magnitude);	//移動到中間

	imshow("Merge Image", _magnitude);	//加上filter的頻譜
	waitKey(0);
	//保存頻譜結果圖
	imwrite(magnitudeName, _magnitude);
	return complexImage;
}



int _3main() {
	/*-------------------------------Spatial Domain--------------------------------------*/
	unsigned char* img = new unsigned char[_size]; // array for image data
	unsigned char* new_arr = new unsigned char[_size]; // array for image data
	unsigned char* img_A = new unsigned char[_size]; // array for image data
	unsigned char* img_B = new unsigned char[_size]; // array for image data

	FILE* file;
	FILE* output_file;
	file = fopen("building_474x632.raw", "rb");
	fread(img, 1, _size, file);

	double sobel0[3][3] = { { -1,-2,-1 }, { 0,0,0 }, {1,2,1} };
	double sobel90[3][3] = { { -1,0,1 }, { -2,0,2 }, {-1,0,1} };


	int** img_darr = new int* [height];	//將原始轉為2維方便filter通過

	for (int i = 0; i < height; i++) {
		img_darr[i] = new int[width];
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			img_darr[i][j] = img[i * width + j];
		}
	}
	img_A = movefilter3x3(img_darr, new_arr, sobel0, 1);
	img_B = movefilter3x3(img_darr, new_arr, sobel90, 1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			new_arr[i * width + j] = abs(img_A[i * width + j]) + abs(img_B[i * width + j]);	//sobel combine
		}
	}
	output_file = fopen("3_spatialSobel.raw", "wb");
	fwrite(new_arr, 1, _size, output_file);

	/*----------------------------------Frenquency Domain--------------------------------------*/

	file = fopen("building_474x632.raw", "rb");
	Mat input(632, 474, CV_8U);

	//原圖存成Mat
	if (fread(input.data, sizeof(unsigned char), 632 * 474, file) != 474 * 632) {
		cout << "Error reading the Baboon raw file." << endl;
		fclose(file);
		return -1;
	}
	fclose(file);

	input.convertTo(input, CV_64F);

	//// Design a 3x3 Sobel filter
	Mat sobelFilterX = (Mat_<float>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
	Mat sobelFilterY = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);

	copyMakeBorder(sobelFilterX, sobelFilterX, 315, 314, 235, 236, BORDER_CONSTANT, 0);
	copyMakeBorder(sobelFilterY, sobelFilterY, 315, 314, 235, 236, BORDER_CONSTANT, 0);

	sobelFilterX.convertTo(sobelFilterX, CV_64F);
	sobelFilterY.convertTo(sobelFilterY, CV_64F);

	string KernelA = "3_Sobel0_Kernel_Frequency.png";
	string KernelB = "3_Sobel90_Kernel_Frequency.png";

	string magnitudeNameA = "3_Sobel0_Magnitude_Frequency.png";
	string magnitudeNameB = "3_Sobel90_Magnitude_Frequency.png";

	string ImageNameA = "3_Sobel0_Frequency.png";
	string ImageNameB = "3_Sobel90_Frequency.png";

	Mat sobelX, sobelY, sobel;
	Mat complexImageA,complexImageB;

	cout << "開始DFT" << endl;
	//dft(sobelFilterX, sobelX, DFT_COMPLEX_OUTPUT);
	sobelX = filterConvert(sobelFilterX);
	sobelY = filterConvert(sobelFilterY);

	sobelX.convertTo(sobelX, CV_64F);
	sobelY.convertTo(sobelY, CV_64F);

	complexImageA = opencv_DFT3(input, sobelX, magnitudeNameA, KernelA);
	complexImageB = opencv_DFT3(input, sobelY, magnitudeNameB, KernelB);

	//IDFT
	Mat inverse;
	Mat to64F;
	FILE* output;
	cout << "開始IDFT" << endl;
	idft(complexImageA, to64F, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	to64F.convertTo(inverse, CV_8U);	//還原回到8bit image
	imshow("Inverse Image Sobel 0", inverse);
	waitKey(0);
	imwrite(ImageNameA, inverse);

	idft(complexImageB, to64F, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	to64F.convertTo(inverse, CV_8U);	//還原回到8bit image
	imshow("Inverse Image Sobel90", inverse);
	waitKey(0);
	imwrite(ImageNameB, inverse);

	return 0;
}