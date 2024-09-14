/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <iostream>
#include <complex>
#include<time.h>

using namespace cv;
using namespace std;


void compare(uchar* original, uchar* inverse, int flag) {
	int sum = 0;
	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((original[i] - inverse[i]), 2);
	}

	double mse = sum / 256 / 256;
	double psnr = 20 * log10(255 / sqrt(mse));
	if (flag == 1) {
		cout << "1_(4) Original Baboon and Inverse Baboons's mse = " << mse << ";    psnr = " << psnr << endl;
	}
	else if (flag == 2) {
		cout << "1_(4) Original Lena and Inverse Lena's mse = " << mse << ";    psnr = " << psnr << endl;
	}
}

// 定義Shift 函數
void shift(cv::Mat& mag) {
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

Mat opencvDFT(Mat InMat, int flag) {
	// 進行DFT
	Mat complexImage;
	dft(InMat, complexImage, DFT_COMPLEX_OUTPUT);

	//// 分離實部和虛部
	Mat planes[2];
	split(complexImage, planes);

	//// 計算幅度頻譜
	Mat magnitude;
	cv::magnitude(planes[0], planes[1], magnitude);

	//// 正規化以在0到255之間顯示
	normalize(magnitude, magnitude, 0, 255, NORM_MINMAX);

	////轉存成8bit資料(0~255)
	Mat _magnitude;
	magnitude.convertTo(_magnitude, CV_8U, 255);

	////移動頻譜至中間位置
	shift(_magnitude);

	// 保存頻譜結果圖
	if (flag == 1) {
		imshow("Baboon's Magnitude Spectrum", _magnitude);
		waitKey(0);
		imwrite("1(3)magnitude_baboon.png", _magnitude);

	}
	else if (flag == 2) {
		imshow("Lena's Magnitude Spectrum", _magnitude);
		waitKey(0);
		imwrite("1(3)magnitude_lena.png", _magnitude);
	}
	return complexImage;
}

int _34main()
{
	double START;
	double END;
	FILE* file_baboon = fopen("baboon_256.raw", "rb");
	unsigned char* arr_baboon = new unsigned char[256 * 256]; // array for baboon

	// 讀取.raw數據並轉換為Mat
	Mat inputBaboon(256, 256, CV_8U);
	if (fread(inputBaboon.data, sizeof(unsigned char), 256 * 256, file_baboon) != 256 * 256) {
		cerr << "Error reading the Baboon raw file." << endl;
		fclose(file_baboon);
		return -1;
	}
	fclose(file_baboon);

	FILE* file_lena = fopen("lena_256.raw", "rb");


	// 讀取.raw數據並轉換為Mat
	Mat inputLena(256, 256, CV_8UC1);
	if (fread(inputLena.data, sizeof(unsigned char), 256 * 256, file_lena) != 256 * 256) {
		cerr << "Error reading the Lena raw file." << endl;
		fclose(file_lena);
		return -1;
	}
	fclose(file_lena);

	Mat dftInput1; //新建轉換所需的容器
	Mat dftInput2; //新建轉換所需的容器
	inputBaboon.convertTo(dftInput1, CV_32F); //將原圖轉換成可計算矩陣
	inputLena.convertTo(dftInput2, CV_32F); //將原圖轉換成可計算矩陣

	/*-------------------------------------------------------------------------------------*/
	// DFT
	START = clock();
	Mat complexImage, complexImage2;
	complexImage = opencvDFT(dftInput1, 1);
	END = clock();
	cout << "1_(3)Baboon_DFT所費時間: " << (END - START) / CLOCKS_PER_SEC << " s." << endl;
	START = clock();
	complexImage2 = opencvDFT(dftInput2, 2);
	END = clock();
	cout << "1_(3)Lena_DFT所費時間: " << (END - START) / CLOCKS_PER_SEC << " s." << endl;
	/*-------------------------------------------------------------------------------------*/
	cout << endl << endl;

	//IDFT
	Mat inverseBaboon, inverseLena;
	Mat to32F;
	FILE* baboon_output;
	FILE* lena_output;

	START = clock();

	idft(complexImage, to32F, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	to32F.convertTo(inverseBaboon, CV_8U);	//從32F還原回到8bit image
	imshow("Baboon's return", inverseBaboon);
	waitKey(0);
	imwrite("1(4)return_baboon.png", inverseBaboon);
	compare(inputBaboon.data, inverseBaboon.data, 1);	//計算MSE

	END = clock();
	cout << "1_(4)Baboonm_IDFT所費時間: " << (END - START) / CLOCKS_PER_SEC <<  " s." << endl;

	/*-------------------------------------------------------------------------------------*/

	START = clock();
	idft(complexImage2, to32F, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	to32F.convertTo(inverseLena, CV_8U);	//為一階陣列，0~255，從32F還原回到8bit image
	imshow("Lena's return", inverseLena);
	waitKey(0);
	imwrite("1(4)return_lena.png", inverseLena);
	compare(inputLena.data, inverseLena.data, 2);	//計算MSE
	END = clock();
	cout << "1_(4)Lena_IDFT所費時間: " << (END - START) / CLOCKS_PER_SEC << " s." << endl;


	return 0;
}