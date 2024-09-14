/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include<cstdlib>
using namespace std;
using namespace cv;
void main()
{
	char input_img[] = "log512.raw";
	char input_img1[] = "2(3)negative.raw";
	char output_img1[] = "3(2)After_log512.raw";
	char output_img2[] = "3(2)After_negativelog512.raw";
	char output_img3[] = "3(3)Match77to153.raw";

	int width = 512;
	int height = 512;
	int size = width * height;
	unsigned char* img = new unsigned char[size];
	unsigned char* img_negative = new unsigned char[size];
	unsigned char* histogram_equlization = new unsigned char[size];
	unsigned char* negative_histogram_equlization = new unsigned char[size];
	unsigned char* match77 = new unsigned char[size];


	int graylevel1[256] = { 0 };
	int graylevel2[256] = { 0 };
	FILE* input_file;
	FILE* output_file;
	/*----------------------------------(1)---------------------------------------*/
	input_file = fopen(input_img, "rb");
	fread(img, 1, size, input_file);
	//統計log512
	for (int i = 0; i < size; i++) {
		int m = img[i];
		graylevel1[m]++;
	}

	input_file = fopen(input_img1, "rb");
	fread(img_negative, 1, size, input_file);
	//統計log512 negative
	for (int i = 0; i < size; i++) {
		int m = img_negative[i];
		graylevel2[m]++;

	}

	Mat image(width, height, CV_8UC1, Scalar(255));
	Mat image2(width, height, CV_8UC1, Scalar(255));
	for (int i = 0; i < width; i++) {
		rectangle(image, Point(2 * i, image.rows - 1), Point(2 * i, image.rows - 1 - graylevel1[i]), Scalar(0));
		rectangle(image2, Point(2 * i, image2.rows - 1), Point(2 * i, image2.rows - 1 - graylevel2[i]), Scalar(0));
	}
	imshow("Histogram of log512.raw", image);
	imwrite("3(1)Histogram_of_log512.png", image);
	imshow("Histogram of log512 negative.raw", image2);
	imwrite("3(1)Histogram_of_negative_log512.png", image2);
	waitKey();
	destroyAllWindows();

	/*----------------------------------(2)---------------------------------------*/
	double cdf_1[256] = { 0 };
	double new_graylavel[256] = { 0 };

	//計算sigma nk 的總和(cdf)
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			cdf_1[i] += graylevel1[j];
		}
	}
	//得出新的graylevel
	for (int i = 0; i < 256; i++) {
		new_graylavel[i] = cdf_1[i] * 255 / size;
	}
	//重新賦予graylevel
	for (int i = 0; i < size; i++) {
		int m = img[i];//取得原本灰階值
		histogram_equlization[i] = new_graylavel[m];	//用新的灰階值填入該pexel
	}

	double count_1[256] = { 0 };

	for (int i = 0; i < size; i++) {
		int m = histogram_equlization[i];
		count_1[m]++;
	}
	output_file = fopen(output_img1, "wb");
	fwrite(histogram_equlization, 1, size, output_file);
	/*------------------------------------(2-negative)----------------------------------------*/
	double cdf_2[256] = { 0 };
	double new_graylavel2[256] = { 0 };

	//計算sigma nk 的總和
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			cdf_2[i] += graylevel2[j];
		}
	}
	//得出新的graylevel
	for (int i = 0; i < 256; i++) {
		new_graylavel2[i] = cdf_2[i] * 255 / size;
		//cout << new_graylavel2[i] << " ";
	}
	//重新賦予graylevel
	for (int i = 0; i < size; i++) {
		int m = img[i];//取得原本灰階值
		negative_histogram_equlization[i] = new_graylavel2[m];	//用新的灰階值填入該pexel
	}

	double count_2[256] = { 0 };

	for (int i = 0; i < size; i++) {
		int m = negative_histogram_equlization[i];
		count_2[m]++;
	}
	output_file = fopen(output_img2, "wb");
	fwrite(negative_histogram_equlization, 1, size, output_file);

	Mat image3(width, height, CV_8UC1, Scalar(255));
	Mat image4(width, height, CV_8UC1, Scalar(255));
	for (int i = 0; i < width; i++) {
		rectangle(image3, Point(2 * i, image3.rows - 1), Point(2 * i, image3.rows - 1 - count_1[i]), Scalar(0));
		rectangle(image4, Point(2 * i, image4.rows - 1), Point(2 * i, image4.rows - 1 - count_2[i]), Scalar(0));
	}
	imshow("Histogram_equalization of log512.raw", image3);
	imwrite("3(2)Afer_log512.png", image3);
	imshow("Histogram of log512 negative.raw", image4);
	imwrite("3(2)Afer_negativelog512.png", image4);
	waitKey();
	destroyAllWindows();
	///*------------------------------------(3)----------------------------------------*/
	double new_graylavel3[256] = { 0 };

	//得出新的graylevel
	for (int i = 0; i < 256; i++) {
		new_graylavel3[i] = (cdf_1[i] * 76 / size)  + 77;
	}
	cout << new_graylavel3[0] << " " << new_graylavel3[255];

	//重新賦予graylevel
	for (int i = 0; i < size; i++) {
		int m = img[i];//取得原本灰階值
		match77[i] = new_graylavel3[m];	//用新的灰階值填入該pexel
	}

	double count_3[256] = { 0 };

	for (int i = 0; i < size; i++) {
		int m = match77[i];
		count_3[m]++;
	}
	output_file = fopen(output_img3, "wb");
	fwrite(match77, 1, size, output_file);

	Mat image5(width, height, CV_8UC1, Scalar(255));
	for (int i = 0; i < width; i++) {
		rectangle(image5, Point(2 * i, image5.rows - 1), Point(2 * i, image5.rows - 1 - count_3[i]), Scalar(0));
	}
	imshow("Histogram of log512.raw", image5);
	imwrite("3(3)Histogram_77to153.png", image5);
	waitKey();
	destroyAllWindows();

	fclose(input_file);
	fclose(output_file);

	delete[] img, histogram_equlization, negative_histogram_equlization;
}