/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

void _22main()
{
	char input_img[] = "log512.raw";
	char output_pow15[] = "2(2)img_pow15.raw";
	char output_pow5[] = "2(2)img_pow5.raw";
	char output_pow02[] = "2(2)img_pow02.raw";
	char output_pow05[] = "2(2)img_pow05.raw";

	int width = 512;
	int height = 512;
	int size = width * height;
	unsigned char* img_original = new unsigned char[size];
	unsigned char* img_pow = new unsigned char[size];

	FILE* input_file;
	FILE* output_file;

	input_file = fopen(input_img, "rb");
	fread(img_original, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		img_pow[i] = pow(int(img_original[i]),15) / pow(255,15) * 255;
		if (int(img_pow[i]) > 255)
			img_pow[i] = 255;
		else if (int(img_pow[i]) < 0)
			img_pow[i] = 0;
	}
	output_file = fopen(output_pow15, "wb");
	fwrite(img_pow, 1, size, output_file);

	for (int i = 0; i < size; i++) {
		img_pow[i] = pow(int(img_original[i]), 5) / pow(255, 5) * 255;
		if (int(img_pow[i]) > 255)
			img_pow[i] = 255;
		else if (int(img_pow[i]) < 0)
			img_pow[i] = 0;
	}
	output_file = fopen(output_pow5, "wb");
	fwrite(img_pow, 1, size, output_file);

	for (int i = 0; i < size; i++) {
		img_pow[i] = pow(int(img_original[i]), 0.2) / pow(255, 0.2) * 255;
		if (int(img_pow[i]) > 255)
			img_pow[i] = 255;
		else if (int(img_pow[i]) < 0)
			img_pow[i] = 0;
	}
	output_file = fopen(output_pow02, "wb");
	fwrite(img_pow, 1, size, output_file);

	for (int i = 0; i < size; i++) {
		img_pow[i] = pow(int(img_original[i]), 0.5) / pow(255, 0.5) * 255;
		if (int(img_pow[i]) > 255)
			img_pow[i] = 255;
		else if (int(img_pow[i]) < 0)
			img_pow[i] = 0;
	}
	output_file = fopen(output_pow05, "wb");
	fwrite(img_pow, 1, size, output_file);

	fclose(input_file);
	fclose(output_file);
	delete[] img_original, img_pow;
}