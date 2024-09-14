/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<cmath>
using namespace std;


int _3main()
{
	char  input_img[] = "lena256.raw";                 // Input  raw image name
	char  input_img_baboon[] = "baboon256.raw";                 // Input  raw image name
	char lena7bit[] = "3_lena7bit.raw";              // Output raw image name
	char lena6bit[] = "3_lena6bit.raw";              // Output raw image name
	char lena5bit[] = "3_lena5bit.raw";              // Output raw image name
	char lena4bit[] = "3_lena4bit.raw";              // Output raw image name
	char lena3bit[] = "3_lena3bit.raw";              // Output raw image name
	char lena2bit[] = "3_lena2bit.raw";              // Output raw image name
	char lena1bit[] = "3_lena1bit.raw";              // Output raw image name
	char baboon7bit[] = "3_baboon7bit.raw";              // Output raw image name
	char baboon6bit[] = "3_baboon6bit.raw";              // Output raw image name
	char baboon5bit[] = "3_baboon5bit.raw";              // Output raw image name
	char baboon4bit[] = "3_baboon4bit.raw";              // Output raw image name
	char baboon3bit[] = "3_baboon3bit.raw";              // Output raw image name
	char baboon2bit[] = "3_baboon2bit.raw";              // Output raw image name
	char baboon1bit[] = "3_baboon1bit.raw";              // Output raw image name

	int width = 256;
	int height = 256;
	int size = width * height;
	float mse = 0;
	float psnr = 0;
	float max = 0;
	double sum = 0;
	int count = 1;

	unsigned char* arr_lena = new unsigned char[256 * 256];
	unsigned char* arr_lena_new = new unsigned char[256 * 256];

	unsigned char* arr_baboon = new unsigned char[256 * 256]; // array for image data原始512資料
	unsigned char* arr_baboon_new = new unsigned char[256 * 256]; // array for image data原始512資料


	int shift = 1;

	FILE* input_file;
	FILE* output_file;

	//open lena----------------------------------------------
	input_file = fopen(input_img, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}


	fread(arr_lena, 1, 256 * 256, input_file);
	int sumoriginal = 0;

	/*----7bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));
	sum = 0;
	cout << count << ": 3_lena7bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	output_file = fopen(lena7bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);
	/*----6bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_lena6bit mse =  " << mse << ";  psnr = " << psnr << endl;
	count++;
	shift++;
	sum = 0;

	output_file = fopen(lena6bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	/*----5bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_lena5bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(lena5bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	/*----4bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_lena4bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(lena4bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	/*----3bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_lena3bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(lena3bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	/*---2bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_lena2bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(lena2bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	/*----1bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_lena_new[i] = arr_lena[i] >> shift;
		arr_lena_new[i] *= pow(2, shift);
		if (arr_lena_new[i] > 127) {
			arr_lena_new[i] = 255;
		}
		else {
			arr_lena_new[i] = 0;
		}

		sum += pow(arr_lena_new[i] - arr_lena[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));
	sum = 0;

	cout << count << ": 3_lena1bit mse =  " << mse << ";  psnr = " << psnr << endl;
	count++;
	output_file = fopen(lena1bit, "wb");
	fwrite(arr_lena_new, 1, 256 * 256, output_file);

	delete[] arr_lena, arr_lena_new;




	/*----open baboon----------------------------------------------------*/
	input_file = fopen(input_img_baboon, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}
	shift = 1;
	cout << endl;
	fread(arr_baboon, 1, 256 * 256, input_file);
	sum = 0;
	/*----7bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon7bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(baboon7bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);
	/*----6bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon6bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;

	output_file = fopen(baboon6bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	/*----5bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon5bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;
	output_file = fopen(baboon5bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	/*----4bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon4bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;
	output_file = fopen(baboon4bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	/*----3bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon3bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;
	output_file = fopen(baboon3bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	/*---2bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboon2bit mse =  " << mse << ";  psnr = " << psnr << endl;
	shift++;
	count++;
	sum = 0;
	output_file = fopen(baboon2bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	/*----1bit-----------------------------------------------------*/
	for (int i = 0; i < height * width; i++) {
		arr_baboon_new[i] = arr_baboon[i] >> shift;
		arr_baboon_new[i] *= pow(2, shift);
		if (arr_baboon_new[i] > 127) {
			arr_baboon_new[i] = 255;
		}
		else {
			arr_baboon_new[i] = 0;
		}
		sum += pow(arr_baboon_new[i] - arr_baboon[i], 2);
	}
	mse = sum / size;
	max = pow(2, 8) - 1;
	psnr = 20 * log10(max / sqrt(mse));

	cout << count << ": 3_baboo1bit mse =  " << mse << ";  psnr = " << psnr << endl;
	output_file = fopen(baboon1bit, "wb");
	fwrite(arr_baboon_new, 1, 256 * 256, output_file);

	delete[] arr_baboon, arr_baboon_new;

	fclose(input_file);
	fclose(output_file);
}
