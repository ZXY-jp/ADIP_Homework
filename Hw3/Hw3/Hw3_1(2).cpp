/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
using namespace std;

int _12main()
{
	char input_dora[] = "doraemon_256.raw";
	char input_lena[] = "lena256.raw";
	char output_img[] = "1(2)Dora_inLena.raw";
	char output_img2[] = "1(2)prove_dorainlena.raw";
	int width = 256;
	int height = 256;
	int size = width * height;
	unsigned char* img_dora = new unsigned char[size];
	unsigned char* img_lena = new unsigned char[size];
	unsigned char* img_dorainlena = new unsigned char[size];
	unsigned char* img_dorainlena_prove = new unsigned char[size];


	FILE* input_file;
	FILE* output_file;


	input_file = fopen(input_lena, "rb");//取出lena
	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	fread(img_lena, 1, size, input_file);//取出dora

	input_file = fopen(input_dora, "rb");
	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	fread(img_dora, 1, size, input_file);

	//把dora藏進lena的LSB
	for (int i = 0; i < size; i++) {
		img_dorainlena[i] = img_lena[i] >> 1;//推掉最小的bit
		img_dorainlena[i] *= 2;	//最小位置補0
		if (int(img_dora[i]) == 255) {
			img_dorainlena[i] += 1;
		}
	}

	output_file = fopen(output_img, "wb");
	fwrite(img_dorainlena, 1, size, output_file);

	//把前面的bit都推掉，僅剩下移動到HSB的LSB，並threhold還原
	for (int i = 0; i < size; i++) {
		img_dorainlena_prove[i] = img_dorainlena[i] << 7;
		if (img_dorainlena_prove[i] > 1) {
			img_dorainlena_prove[i] = 255;
		}
		else {
			img_dorainlena_prove[i] = 0;
		}
	}

	output_file = fopen(output_img2, "wb");
	fwrite(img_dorainlena_prove, 1, size, output_file);

	fclose(input_file);
	fclose(output_file);
	delete[] img_dora, img_dora, img_dorainlena;
}