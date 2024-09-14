/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
using namespace std;
void _1main()
{
	char input_dora[] = "doraemon_256.raw";
	char output_dora[] = "1(1)Doraemon_256_binarize.raw";
	int width = 256;
	int height = 256;
	int size = width * height;
	unsigned char* img_dora = new unsigned char[size];
	unsigned char* img_dora_bi = new unsigned char[size];

	FILE* input_file;
	FILE* output_file;

	input_file = fopen(input_dora, "rb");
	fread(img_dora, 1, size, input_file);

	for (int i = 0; i < size; i++) {
		if (int(img_dora[i]) > 1) {
			img_dora_bi[i] = 255;
			
		}
		else {
			img_dora_bi[i] = 0;
		}
	}

	output_file = fopen(output_dora, "wb");
	fwrite(img_dora_bi, 1, size, output_file);
	fclose(input_file);
	fclose(output_file);
	delete[] img_dora, img_dora_bi;
}