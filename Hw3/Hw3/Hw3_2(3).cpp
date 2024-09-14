/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

void _23main()
{
	char input_img[] = "log512.raw";
	char input_log[] = "2(1)img_log.raw";
	char input_ilog[] = "2(1)img_inverselog.raw";
	char input_pow02[] = "2(2)img_pow02.raw";
	char input_pow05[] = "2(2)img_pow05.raw";
	char input_pow5[] = "2(2)img_pow5.raw";
	char input_pow15[] = "2(2)img_pow15.raw";

	char output_negative[] = "2(3)negative.raw";
	char output_negative_log[] = "2(3)negative_log.raw";
	char output_negative_inverselog[] = "2(3)negative_inverse_log.raw";
	char output_negative_pow5[] = "2(3)negative_pow5.raw";
	char output_negative_pow15[] = "2(3)negative_pow15.raw";
	char output_negative_pow02[] = "2(3)negative_pow02.raw";
	char output_negative_pow05[] = "2(3)negative_pow05.raw";
	int width = 512;
	int height = 512;
	int size = width * height;
	unsigned char* img_original = new unsigned char[size];
	unsigned char* img_negative = new unsigned char[size];

	FILE* input_file;
	FILE* output_file;

	/*---------------------original---------------------*/
	input_file = fopen(input_img, "rb");
	fread(img_original, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative, "wb");
	fwrite(img_negative, 1, size, output_file);

	/*---------------------log---------------------*/
	input_file = fopen(input_log, "rb");
	fread(img_original, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_log, "wb");
	fwrite(img_negative, 1, size, output_file);
	/*---------------------inverselog---------------------*/
	input_file = fopen(input_ilog, "rb");
	fread(img_original, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_inverselog, "wb");
	fwrite(img_negative, 1, size, output_file);
	/*---------------------pow02---------------------*/
	input_file = fopen(input_pow02, "rb");
	fread(img_original, 1, size, input_file);
	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_pow02, "wb");
	fwrite(img_negative, 1, size, output_file);
	/*---------------------pow05---------------------*/
	input_file = fopen(input_pow05, "rb");
	fread(img_original, 1, size, input_file);
	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_pow05, "wb");
	fwrite(img_negative, 1, size, output_file);
	/*---------------------pow5---------------------*/
	input_file = fopen(input_pow5, "rb");
	fread(img_original, 1, size, input_file);
	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_pow5, "wb");
	fwrite(img_negative, 1, size, output_file);
	/*---------------------pow15---------------------*/
	input_file = fopen(input_pow15, "rb");
	fread(img_original, 1, size, input_file);
	for (int i = 0; i < size; i++) {
		img_negative[i] = 255 - 1 - img_original[i];
		if (int(img_negative[i]) > 255)
			img_negative[i] = 255;
		else if (int(img_negative[i]) < 0)
			img_negative[i] = 0;
	}
	output_file = fopen(output_negative_pow15, "wb");
	fwrite(img_negative, 1, size, output_file);


	fclose(input_file);
	fclose(output_file);
	delete[] img_original, img_negative;
}