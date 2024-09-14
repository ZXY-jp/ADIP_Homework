/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

void _21main()
{
	char input_img[] = "log512.raw";
	char output_log[] = "2(1)img_log.raw";
	char output_inverselog[] = "2(1)img_inverselog.raw";

	int width = 512;
	int height = 512;
	int size = width * height;
	unsigned char* img_original = new unsigned char[size];
	unsigned char* img_log = new unsigned char[size];
	unsigned char* img_inverselog = new unsigned char[size];

	FILE* input_file;
	FILE* output_file;

	input_file = fopen(input_img, "rb");
	fread(img_original, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		img_log[i] = log(1 + int(img_original[i])) / log(255) * 255;
		if (int(img_log[i]) > 255)
			img_log[i] = 255;
		else if (int(img_log[i]) < 0)
			img_log[i] = 0;
	}
	for (int i = 0; i < size; i++) {
		img_inverselog[i] = exp(1 + int(img_original[i])) / exp(255) * 255;
		img_inverselog[i] = img_inverselog[i] % 255;
		if (int(img_inverselog[i]) < 0)
			img_inverselog[i] = 0;
		cout << int(img_original[i]) << " ";
	}

	output_file = fopen(output_log, "wb");
	fwrite(img_log, 1, size, output_file);

	output_file = fopen(output_inverselog, "wb");
	fwrite(img_inverselog, 1, size, output_file);

	fclose(input_file);
	fclose(output_file);
	delete[] img_original, img_log, img_inverselog;
}