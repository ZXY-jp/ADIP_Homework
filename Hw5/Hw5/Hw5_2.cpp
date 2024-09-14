/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;
using namespace cv;
#define width 256
#define height 256

double distance(int i, int j) {
	//P=2*width, Q = 2*height
	return sqrt(pow(i - width / 2, 2) + pow(j - height / 2, 2));
}

// 定義Shift 函數
void _shift(Mat& mag) {
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

void _opencv_DFT(Mat InMat, Mat filter, string magnitudeName, string filename, int flag) {
	// 進行DFT
	Mat complexImage;

	dft(InMat, complexImage, DFT_COMPLEX_OUTPUT);
	_shift(filter);	//將原先移動道邊邊過的filter移動到中間

	Mat planes[2];
	split(complexImage, planes);// 分離實部和虛部

	//進行filter相乘
	cout << complexImage.size << " " << filter.size << endl;
	cout << complexImage.depth() << " " << filter.depth() << endl;
	multiply(planes[0], filter, planes[0]);
	multiply(planes[1], filter, planes[1]);
	cv::merge(planes, 2, complexImage);

	//// 計算幅度頻譜
	Mat magnitude;
	cv::magnitude(planes[0], planes[1], magnitude);

	//// 正規化以在0到255之間顯示

	normalize(magnitude, magnitude, 0, pow(255, flag), NORM_MINMAX);	//LPF 要255 HPF要1

	////轉存成8bit資料(0~255)
	Mat _magnitude;
	magnitude.convertTo(_magnitude, CV_8U, 255);
	_shift(_magnitude);	//將原先移動道邊邊過的filter移動到中間

	//imshow("Merge Image", _magnitude);	//加上filter的頻譜
	//waitKey(0);

	// 保存頻譜結果圖
	imwrite(magnitudeName, _magnitude);


	/*---------------------IDFT----------------------*/
	Mat inverseDFT;

	idft(complexImage, inverseDFT, DFT_REAL_OUTPUT | DFT_SCALE);
	for (int u = 0; u < height; u++) {
		for (int v = 0; v < width; v++) {
			cout << inverseDFT.at<double>(u, v) << " ";
		}
		cout << endl;
	}
	Mat restoredImage;
	inverseDFT.convertTo(restoredImage, CV_8U);	//為一階陣列，0~255，從32F還原回到8bit image

	imshow("Inverse Image", restoredImage);
	waitKey(0);

	// 保存還原結果圖
	imwrite(filename, restoredImage);
}


int main()
{
	FILE* file_baboon;
	file_baboon = fopen("baboon_256.raw", "rb");
	int n;
	int D0[] = { 10,20,40,100 };
	int type;

	// 讀取.raw數據並轉換為Mat
	Mat inputBaboon(256, 256, CV_8U);
	if (fread(inputBaboon.data, sizeof(unsigned char), 256 * 256, file_baboon) != 256 * 256) {
		cout << "Error reading the Baboon raw file." << endl;
		fclose(file_baboon);
		return -1;
	}
	fclose(file_baboon);

	cout << "Enter 1 for Gaussian, 2 for Butterworth, 3 for Ideal" << endl;
	while (cin >> type) {
		/*cout << "Enter D0: 10 or 20 or 40 or 100" << endl;
		cin >> D0;*/
		string str0 = "2(";
		string str1, str2;
		string str3 = "_magnitude.png";
		string str4 = "_returnImage.png";
		string str5 = "_kernel.png";

		string LPFmagnitudeName, LPFImageName, HPFmagnitudeName, HPFImageName, L_kernelName, H_kernelName;

		Mat temp32; //新建轉換所需的容器
		inputBaboon.convertTo(temp32, CV_64F); //將原圖轉換成可計算矩陣


		Mat LPF(height, width, CV_64F);
		Mat HPF(height, width, CV_64F);

		double H = 0;
		double D = 0;

		//afterDFT.convertTo(afterDFT, CV_64F); //將原圖轉換成可計算矩陣

		switch (type) {
		case 1:
			for (int k = 0; k < 4; k++) {
				LPF.convertTo(LPF, CV_64F);
				HPF.convertTo(HPF, CV_64F);

				cout << "Gaussian" << endl;
				str1 = ")GaussianLPF_";
				str2 = ")GaussianHPF_";

				LPFmagnitudeName = str0 + to_string(type) + str1 + to_string(D0[k]) + str3;
				LPFImageName = str0 + to_string(type) + str1 + to_string(D0[k]) + str4;
				HPFmagnitudeName = str0 + to_string(type) + str2 + to_string(D0[k]) + str3;
				HPFImageName = str0 + to_string(type) + str2 + to_string(D0[k]) + str4;
				L_kernelName = str0 + to_string(type) + str1 + to_string(D0[k]) + str5;
				H_kernelName = str0 + to_string(type) + str2 + to_string(D0[k]) + str5;

				//Gaussian filter
				for (int i = 0; i < 256; i++) {
					for (int j = 0; j < 256; j++) {
						D = distance(i, j);
						H = exp(-(D * D) / (2 * D0[k] * D0[k]));
						LPF.at<double>(i, j) = H;
						HPF.at<double>(i, j) = 1 - H;
					}
				}
				_opencv_DFT(temp32, LPF, LPFmagnitudeName, LPFImageName, 1);
				_shift(LPF);	//移回到中間
				//imshow("Filter's Magnitude Spectrum", LPF);
				//waitKey(0);

				// 保存filter頻譜結果圖
				LPF.convertTo(LPF, CV_8U, 255);
				imwrite(L_kernelName, LPF);
				cout << " LPF Saved Success" << endl;

				_opencv_DFT(temp32, HPF, HPFmagnitudeName, HPFImageName, 0);
				_shift(HPF);	//移回到中間
				//imshow("Filter's Magnitude Spectrum", HPF);
				//waitKey(0);

				// 保存filter頻譜結果圖
				HPF.convertTo(HPF, CV_8U, 255);
				imwrite(H_kernelName, HPF);
				cout << " HPF Saved Success" << endl;
			}

			break;
		case 2:
			for (int k = 0; k < 4; k++) {
				//Butterworth
				for (int n = 1; n <= 4; n++) {
					LPF.convertTo(LPF, CV_64F);
					HPF.convertTo(HPF, CV_64F);

					cout << "Gaussian" << endl;
					str1 = ")ButterworthLPF_";
					str2 = ")ButterworthHPF_";

					LPFmagnitudeName = str0 + to_string(type) + str1 + to_string(D0[k]) + "_n" + to_string(n) + str3;
					LPFImageName = str0 + to_string(type) + str1 + to_string(D0[k]) + "_n" + to_string(n) + str4;
					HPFmagnitudeName = str0 + to_string(type) + str2 + to_string(D0[k]) + "_n" + to_string(n) + str3;
					HPFImageName = str0 + to_string(type) + str2 + to_string(D0[k]) + "_n" + to_string(n) + str4;
					L_kernelName = str0 + to_string(type) + str1 + to_string(D0[k]) + "_n" + to_string(n) + str5;
					H_kernelName = str0 + to_string(type) + str2 + to_string(D0[k]) + "_n" + to_string(n) + str5;

					// Butterworth
					for (int i = 0; i < 256; i++) {
						for (int j = 0; j < 256; j++) {
							D = distance(i, j);
							LPF.at<double>(i, j) = 1 / (1 + pow(D / D0[k], 2 * n));
							HPF.at<double>(i, j) = 1 / (1 + pow(D0[k] / D, 2 * n));
						}
					}
					_opencv_DFT(temp32, LPF, LPFmagnitudeName, LPFImageName, 1);
					_shift(LPF);	//移回到中間
					//imshow("Filter's Magnitude Spectrum", LPF);
					//waitKey(0);

					// 保存filter頻譜結果圖
					LPF.convertTo(LPF, CV_8U, 255);
					imwrite(L_kernelName, LPF);
					cout << " LPF Saved Success" << endl;

					_opencv_DFT(temp32, HPF, HPFmagnitudeName, HPFImageName, 0);
					_shift(HPF);	//移回到中間
					//imshow("Filter's Magnitude Spectrum", HPF);
					//waitKey(0);

					// 保存filter頻譜結果圖
					HPF.convertTo(HPF, CV_8U, 255);
					imwrite(H_kernelName, HPF);
					cout << " HPF Saved Success" << endl;
				}
			}
			break;
		case 3:
			for (int k = 0; k < 4; k++) {
				LPF.convertTo(LPF, CV_64F);
				HPF.convertTo(HPF, CV_64F);
				str1 = ")Ideal_LPF_";
				str2 = ")Ideal_HPF_";

				LPFmagnitudeName = str0 + to_string(type) + str1 + to_string(D0[k]) + str3;
				LPFImageName = str0 + to_string(type) + str1 + to_string(D0[k]) + str4;
				HPFmagnitudeName = str0 + to_string(type) + str2 + to_string(D0[k]) + str3;
				HPFImageName = str0 + to_string(type) + str2 + to_string(D0[k]) + str4;
				L_kernelName = str0 + to_string(type) + str1 + to_string(D0[k]) + str5;
				H_kernelName = str0 + to_string(type) + str2 + to_string(D0[k]) + str5;

				//Ideal filter
				for (int i = 0; i < 256; i++) {
					for (int j = 0; j < 256; j++) {
						D = distance(i, j);
						if (D <= D0[k]) {
							LPF.at<double>(i, j) = 1;
							HPF.at<double>(i, j) = 0;
						}
						else {
							LPF.at<double>(i, j) = 0;
							HPF.at<double>(i, j) = 1;
						}
					}
				}
				_opencv_DFT(temp32, LPF, LPFmagnitudeName, LPFImageName, 1);
				_shift(LPF);	//移回到中間
				//imshow("Filter's Magnitude Spectrum", LPF);
				//waitKey(0);

				// 保存filter頻譜結果圖
				LPF.convertTo(LPF, CV_8U, 255);
				imwrite(L_kernelName, LPF);
				cout << " LPF Saved Success" << endl;

				_opencv_DFT(temp32, HPF, HPFmagnitudeName, HPFImageName, 0);
				_shift(HPF);	//移回到中間
				//imshow("Filter's Magnitude Spectrum", HPF);
				//waitKey(0);

				// 保存filter頻譜結果圖
				HPF.convertTo(HPF, CV_8U, 255);
				imwrite(H_kernelName, HPF);
				cout << " HPF Saved Success" << endl;
			}
			break;
		default:
			cout << "Invalid input!" << endl;
			exit;
		}
		cout << "Enter 1 for Gaussian, 2 for Butterworth, 3 for Ideal, ctrl + z to exit." << endl;

	}

	/*-------------------------------------------------------------------------------------*/
	return 0;

}