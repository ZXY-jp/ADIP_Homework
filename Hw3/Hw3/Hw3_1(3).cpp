/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <string>
#include<cstdlib>
using namespace std;

void dora_in_lena(unsigned char* lena, unsigned char* dora, int flag) {
	int size = 256 * 256;
	unsigned char* img_dorainlena = new unsigned char[size];

	char output_img[] = "1(3)Dora_inLena_1bitplane.raw";
	char output_img2[] = "1(3)Dora_inLena_2bitplane.raw";
	char output_img3[] = "1(3)Dora_inLena_3bitplane.raw";
	char output_img4[] = "1(3)Dora_inLena_4bitplane.raw";
	char output_img5[] = "1(3)Dora_inLena_5bitplane.raw";
	char output_img6[] = "1(3)Dora_inLena_6bitplane.raw";
	char output_img7[] = "1(3)Dora_inLena_7bitplane.raw";
	cout << "Dora hide in " << flag << "'s bit plane in lena." << endl;

	FILE* output_file;
	for (int x = 0; x < size; x++) {
		int dora_mask = 0;
		if (int(dora[x])== 255) {
			dora_mask = 1 << flag;
		}
		if (x == 0)
			cout << dora_mask << endl;
		img_dorainlena[x] = dora_mask | lena[x] ;
	}
	switch (flag) {
	case 1:
		output_file = fopen(output_img, "wb");
		fwrite(img_dorainlena, 1, size, output_file);
		break;

	case 2:
		output_file = fopen(output_img2, "wb");	
		fwrite(img_dorainlena, 1, size, output_file);

		break;
	case 3:
		output_file = fopen(output_img3, "wb");	
		fwrite(img_dorainlena, 1, size, output_file);

		break;
	case 4:
		output_file = fopen(output_img4, "wb");	
		fwrite(img_dorainlena, 1, size, output_file);
		break;
	case 5:
		output_file = fopen(output_img5, "wb");
		fwrite(img_dorainlena, 1, size, output_file);
		break;
	case 6:
		output_file = fopen(output_img6, "wb");
		fwrite(img_dorainlena, 1, size, output_file);
		break;
	case 7:
		output_file = fopen(output_img7, "wb");
		fwrite(img_dorainlena, 1, size, output_file);
		break;
	}
	delete[] img_dorainlena;
}

void _13main()
{
	char input_dora[] = "doraemon_256.raw";
	char input_lena[] = "lena256.raw";
	char output_img[] = "1(3)Dora_inLena_1bitplane.raw";
	char output_img2[] = "1(3)Dora_inLena_2bitplane.raw";
	char output_img3[] = "1(3)Dora_inLena_3bitplane.raw";
	char output_img4[] = "1(3)Dora_inLena_4bitplane.raw";
	char output_img5[] = "1(3)Dora_inLena_5bitplane.raw";
	char output_img6[] = "1(3)Dora_inLena_6bitplane.raw";
	char output_img7[] = "1(3)Dora_inLena_7bitplane.raw";

	int width = 256;
	int height = 256;
	int size = width * height;
	unsigned char* img_dora = new unsigned char[size];
	unsigned char* img_lena = new unsigned char[size];


	FILE* input_file;


	input_file = fopen(input_lena, "rb");//取出lena
	fread(img_lena, 1, size, input_file);

	input_file = fopen(input_dora, "rb");//取出dora
	fread(img_dora, 1, size, input_file);

	//把dora藏進lenabit plane
	for (int i = 1; i < 8; i++) {
		dora_in_lena(img_lena, img_dora, i);
	}

	fclose(input_file);
	delete[] img_dora, img_dora;
}