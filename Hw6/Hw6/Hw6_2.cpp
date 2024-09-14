/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <iostream>
#include <complex>
#include <string>
#include <math.h>

using namespace std;
using namespace cv;
#define width 256
#define height 256
#define PI 3.1415926

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

Mat cv_DFT(Mat InMat) {
	// 進行DFT
	InMat.convertTo(InMat, CV_64FC2);
	Mat complexImage(256, 256, CV_64FC2);
	dft(InMat, complexImage, DFT_COMPLEX_OUTPUT);
	return complexImage;
}

Mat makeH() {
	Mat bigH(256, 256, CV_64FC2);
	double a = -0.07;
	double b = 0.07;
	double T = 1;
	const complex<double> j(0, 1);

	for (int u = 0; u < height; u++) {
		for (int v = 0; v < width; v++) {
			double sq = PI * (u * a + v * b);
			if (sq != 0) {
				bigH.at<complex<double>>(u, v) = T / (sq)*sin(sq) * exp(-j * sq);
			}
			else {
				bigH.at<complex<double>>(u, v) = (1, 0);
			}
		}
	}
	return bigH;
}

Mat InverseFilter(Mat InMat, int r, bool noise) {
	Mat bigH(256, 256, CV_64FC2);
	bigH = makeH();

	Mat complexImage(256, 256, CV_64FC2);
	Mat LPF(height, width, CV_64F);
	int n = 3;
	int count = 0;

	// Butter Worth
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			double D = sqrt(pow(i - width / 2, 2) + pow(j - height / 2, 2));
			LPF.at<double>(i, j) = 1 / (1 + pow(D / r, 2 * n));
		}
	}

	double limit;
	limit = 0.025;
	for (int u = 0; u < height; u++) {
		for (int v = 0; v < width; v++) {
			if (abs(bigH.at<complex<double>>(u, v)) >= limit) {
				complexImage.at<complex<double>>(u, v) = InMat.at<complex<double>>(u, v) / bigH.at<complex<double>>(u, v);	//F_hat = G / H
			}
			else {
				complexImage.at<complex<double>>(u, v) = InMat.at<complex<double>>(u, v) / complex<double>(1, 0);  	//若H太極端，僅讓它通過LPF不除H
			}
		}
	}

	Mat planes[2];
	shift(LPF);
	split(complexImage, planes);
	multiply(planes[0], LPF, planes[0]);
	multiply(planes[1], LPF, planes[1]);
	cv::merge(planes, 2, complexImage);

	return complexImage;
}

Mat Wiener(Mat InMat, double K) {

	Mat bigH(256, 256, CV_64FC2);
	bigH = makeH();
	Mat complexImage(256, 256, CV_64FC2);
	Mat LPF(height, width, CV_64F);
	int n = 3;
	int count = 0;
	double limit = 0.025;


	for (int u = 0; u < height; u++) {
		for (int v = 0; v < width; v++) {
			complex<double> H = bigH.at<complex<double>>(u, v);
			if (abs(bigH.at<complex<double>>(u, v)) >= limit) {
				complexImage.at<complex<double>>(u, v) = InMat.at<complex<double>>(u, v) * ((abs(H) * abs(H)) / ((abs(H) * abs(H)) + K) / H);
			}
			else {
				complexImage.at<complex<double>>(u, v) = InMat.at<complex<double>>(u, v);
			}
		}
	}

	return complexImage;
}

Mat Constrained_LS(Mat InMat, int r, double gamma) {

	Mat bigH(256, 256, CV_64FC2);
	Mat paddP(256, 256, CV_64FC2);
	bigH = makeH();
	Mat  laplacian = (Mat_<float>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
	copyMakeBorder(laplacian, laplacian, 127, 126, 127, 126, BORDER_CONSTANT, 0);
	//dft(laplacian, paddP, DFT_COMPLEX_OUTPUT);
	paddP = cv_DFT(laplacian);

	Mat complexImage(256, 256, CV_64FC2);
	int n = 3;
	int count = 0;
	double limit = 0.145;
	for (int u = 0; u < height; u++) {
		for (int v = 0; v < width; v++) {
			complex<double> H = bigH.at<complex<double>>(u, v);
			complex<double> I = InMat.at<complex<double>>(u, v);

			if ((abs(bigH.at<complex<double>>(u, v)) >= limit)) {
				complex<double> H_star = conj(H);	//取得共軛
				complex<double> P = paddP.at<complex<double>>(u, v);
				complexImage.at<complex<double>>(u, v) = (H_star / (abs(H) * abs(H) + gamma * (abs(P) * abs(P)))) * I;
			}
			else {
				complexImage.at<complex<double>>(u, v) = I;
			}

		}
	}

	return complexImage;
}


void Image_compare(uchar* original, uchar* deblur, int flag, int type) {
	int sum = 0;
	for (int i = 0; i < 256 * 256; i++) {
		sum += pow((original[i] - deblur[i]), 2);
	}

	double mse = sum / 256 / 256;
	double psnr = 20 * log10(255 / sqrt(mse));
	if (flag == 1) {
		cout << "2_(" << type << ") Original Lena and deblur Lena's mse = " << mse << ";    psnr = " << psnr << endl;
	}
	else if (flag == 2) {
		cout << "2_(" << type << ") Original Noise Lena and deblur Noise Lena's mse = " << mse << ";    psnr = " << psnr << endl;
	}
	cout << "----------------------------------------------------------" << endl << endl;
}


Mat cv_IDFT(Mat complexImage, string filename) {
	Mat inverseDFT;
	idft(complexImage, inverseDFT, DFT_REAL_OUTPUT + DFT_SCALE);
	normalize(inverseDFT, inverseDFT, 0, 255, NORM_MINMAX);
	inverseDFT.convertTo(inverseDFT, CV_8U);	//為一階陣列，0~255，還原回到8bit image
	imshow("Inverse Image", inverseDFT);
	waitKey(0);
	destroyAllWindows();

	//// 保存還原結果圖
	imwrite(filename, inverseDFT);
	return inverseDFT;
}

int main()
{
	FILE* input_file;
	input_file = fopen("lena_256_blur.raw", "rb");

	// 讀取blur lena .raw數據並轉換為Mat
	Mat lena_blur(256, 256, CV_8U);
	if (fread(lena_blur.data, sizeof(unsigned char), 256 * 256, input_file) != 256 * 256) {
		cout << "Error reading the raw file." << endl;
		fclose(input_file);
		return -1;
	}

	input_file = fopen("lena_256_blur_noise.raw", "rb");
	// 讀取 lena_256 .raw數據並轉換為Mat
	Mat lena_blur_noise(256, 256, CV_8U);
	if (fread(lena_blur_noise.data, sizeof(unsigned char), 256 * 256, input_file) != 256 * 256) {
		cout << "Error reading the raw file." << endl;
		fclose(input_file);
		return -1;
	}

	input_file = fopen("lena_256.raw", "rb");
	// 讀取 lena_256 .raw數據並轉換為Mat
	Mat lena_origin(256, 256, CV_8U);
	if (fread(lena_origin.data, sizeof(unsigned char), 256 * 256, input_file) != 256 * 256) {
		cout << "Error reading the raw file." << endl;
		fclose(input_file);
		return -1;
	}

	fclose(input_file);


	int type;
	cout << "Enter 1 for Inverse Filtering, 2 for Wiener Filtering , 3 for Constrained Least-Square Filter" << endl;
	while (cin >> type) {
		string str0 = "2(";
		string str1, str2, str4;
		string str3 = "_deblur.png";

		string lenaName, noiseName, lenaName2, noiseName2;

		double H = 0;
		double D = 0;
		double k[3] = { 0.005, 0.05, 0.1 };
		double gamma[3] = { 0.02,0.5,10 };

		int r = 120;
		int count = 1;
		Mat lena_dft, lena_deblur;

		switch (type) {
		case 1:
			str1 = ")Lena_r";
			str2 = ")Lena_noise_r";

			for (int z = 0; z < 3; z++) {
				lenaName = str0 + to_string(type) + str1 + to_string(r) + str3;
				cout << count << ". without Gaussian, R = " << r << endl;
				lena_dft = cv_DFT(lena_blur);
				//Inverse Filter
				lena_deblur = InverseFilter(lena_dft, r, false);
				cv_IDFT(lena_deblur, lenaName);

				Image_compare(lena_origin.data, lena_deblur.data, 1, type);
				count++;

				noiseName = str0 + to_string(type) + str2 + to_string(r) + str3;
				cout << count << ". with Gaussian, R = " << r << endl;
				lena_dft = cv_DFT(lena_blur_noise);
				//Inverse Filter
				lena_deblur = InverseFilter(lena_dft, r, true);
				cv_IDFT(lena_deblur, noiseName);
				Image_compare(lena_origin.data, lena_deblur.data, 2, type);
				count++;
				r -= 40;
			}

			break;

		case 2:
			str1 = ")Lena_k";
			str2 = ")Lena_noise_k";

			count = 1;
			for (int a = 0; a < 3; a++) {
				lenaName = str0 + to_string(type) + str1 + to_string(k[a]) + str3;
				cout << count << ".without Gaussian, K = " << k[a] << endl;

				lena_dft = cv_DFT(lena_blur);
				lena_deblur = Wiener(lena_dft, k[a]);	//Inverse Filter
				lena_deblur = cv_IDFT(lena_deblur, lenaName);
				Image_compare(lena_origin.data, lena_deblur.data, 1, type);
				count++;

				noiseName = str0 + to_string(type) + str2 + to_string(k[a]) + str3;
				cout << count << ". with Gaussian, K = " << k[a] << endl;
				lena_dft = cv_DFT(lena_blur_noise);
				//Inverse Filter
				lena_deblur = Wiener(lena_dft, k[a]);
				cv_IDFT(lena_deblur, noiseName);
				Image_compare(lena_origin.data, lena_deblur.data, 1, type);
				count++;
			}

			break;

		case 3:
			str1 = ")Lena_γ";
			str2 = ")Lena_noise_γ";
			for (int i = 0; i < 3; i++) {
				lenaName = str0 + to_string(type) + str1 + to_string(gamma[i]) + str3;
				cout << count << ".without Gaussian, γ = " << gamma[i] << endl;

				lena_dft = cv_DFT(lena_blur);
				lena_deblur = Constrained_LS(lena_dft, r, gamma[i]);	//Inverse Filter
				lena_deblur = cv_IDFT(lena_deblur, lenaName);
				Image_compare(lena_origin.data, lena_deblur.data, 1, type);
				count++;

				noiseName = str0 + to_string(type) + str2 + to_string(gamma[i]) + str3;
				cout << count << ". with Gaussian, γ = " << gamma[i] << endl;
				lena_dft = cv_DFT(lena_blur_noise);
				//Inverse Filter
				lena_deblur = Constrained_LS(lena_dft, r, gamma[i]);
				cv_IDFT(lena_deblur, noiseName);
				Image_compare(lena_origin.data, lena_deblur.data, 1, type);
				count++;
			}
			break;

		default:
			cout << "Invalid input!" << endl;
			exit;
		}
		cout << "Enter 1 for Inverse Filtering, 2 for Wiener Filtering , 3 for Constrained Least-Square Filter" << endl;

	}

	///*-------------------------------------------------------------------------------------*/
	return 0;
}