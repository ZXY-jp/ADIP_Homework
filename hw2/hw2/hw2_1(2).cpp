/*¬°¨Ï­ÓÀÉ®×¤£¤¬¬Û¤zÂZ¡A
¦bmain«e³£¦³¥[¤W_(ÃD¸¹)§@°Ï§O¡A
°õ¦æ«e¶·¥ý±N²Å¸¹§R°£*/

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>
using namespace std;


int _12main()
{

	char  input_img[] = "lena512.raw";                 // Input  raw image name
	char  input_blurimg[] = "lena512_xnview_blur.raw";                 // Input blur raw image name
	char shrink_lena_to_128a[] = "1(2)_shrink_lena_to_128.raw";              // Output raw image name
	char shrink_lena_to_128b[] = "1(2)_shrink_lena_to_128withblur.raw";              // Output raw image name

	int width = 512;
	int height = 512;
	int size = width * height;

	unsigned char* img_lena = new unsigned char[512 * 512];
	unsigned char* blur_lena = new unsigned char[512 * 512]; // array for image data­ì©l512¸ê®Æ
	unsigned char* my_shrink_lena = new unsigned char[128 * 128]; // array for image data §Ú¥Í¦¨ªº128

	FILE* input_file;
	FILE* output_file;


	//without blur----------------------------------------------
	input_file = fopen(input_img, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	fread(img_lena, 1, 512*512, input_file);

	for (int i = 0; i < 128; i += 1) {
		for (int j = 0; j < 128; j += 1) {
			int originX = j / 0.25;
			int originY = i / 0.25;
			originX = max(0, min(originX, 512 - 1));
			originY = max(0, min(originY, 512 - 1));
			my_shrink_lena[i * 128 + j] = img_lena[originY * 512 + originX];

		}
	}
	output_file = fopen(shrink_lena_to_128a, "wb");
	fwrite(my_shrink_lena, 1, 128 * 128, output_file);


	//---with blur-----------------------------------
	input_file = fopen(input_blurimg, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	fread(blur_lena, 1, 512*512, input_file);

	for (int i = 0; i < 128; i += 1) {
		for (int j = 0; j < 128; j += 1) {
			int originX = j / 0.25;
			int originY = i / 0.25;
			originX = max(0, min(originX, 512 - 1));
			originY = max(0, min(originY, 512 - 1));
			my_shrink_lena[i * 128 + j] = blur_lena[originY * 512 + originX];
		}
	}
	output_file = fopen(shrink_lena_to_128b, "wb");
	fwrite(my_shrink_lena, 1, 128 * 128, output_file);

	delete[] img_lena, blur_lena, my_shrink_lena;

	
	fclose(input_file);
	fclose(output_file);
}


/*----------灰階doraemon並儲存為raw---------------*/
// #include <opencv2/opencv.hpp>
// #include <fstream>
// using namespace cv;
// using namespace std;
// int main()
// {
// 	int width = 512;
// 	int height = 512;
// 	// 建立畫布，Scalar 設定色彩(BGR)
// 	Mat image(width, height, CV_8UC1, Scalar(255));


// 	Point center(width / 2, height / 2);
// 	Point nose(width / 2, height / 2 + 10);

// 	circle(image, center, 201, Scalar(120), -1);	// 臉型
// 	ellipse(image, Point(width / 2, height / 2 + 75), Size(220, height / 4), 0, 0, 360, Scalar(255), -1);	//嘴部橢圓底色
// 	ellipse(image, Point(width / 2, height / 2 + 75), Size(220, height / 4), 0, 207, 333, Scalar(0), 3);		//嘴部橢圓邊界
// 	ellipse(image, Point(width / 2 - 51, height / 2 - 50), Size(width / 10, height / 8), 0, 0, 360, Scalar(255), -1);		//左眼白橢圓底色
// 	ellipse(image, Point(width / 2 - 51, height / 2 - 50), Size(width / 10, height / 8), 0, 0, 360, Scalar(0), 2);		//左眼白橢圓邊界
// 	ellipse(image, Point(width / 2 + 51, height / 2 - 50), Size(width / 10, height / 8), 0, 0, 360, Scalar(255), -1);		//右眼白橢圓底色
// 	ellipse(image, Point(width / 2 + 51, height / 2 - 50), Size(width / 10, height / 8), 0, 0, 360, Scalar(0), 2);		//右眼白橢圓邊界

// 	ellipse(image, Point(width / 2 - 30, height / 2 - 40), Size(width / 22, height / 18), 0, 0, 360, Scalar(0), -1);		//左眼瞳孔
// 	ellipse(image, Point(width / 2 + 30, height / 2 - 40), Size(width / 22, height / 18), 0, 0, 360, Scalar(0), -1);		//右眼瞳孔
// 	ellipse(image, Point(width / 2 - 30, height / 2 - 40), Size(width / 50, height / 48), 0, 0, 360, Scalar(255), -1);		//左眼反光
// 	ellipse(image, Point(width / 2 + 30, height / 2 - 40), Size(width / 50, height / 48), 0, 0, 360, Scalar(255), -1);		//右眼反光

// 	//嘴巴&鬍鬚
// 	ellipse(image, Point(width / 2, height / 2 + 60), Size(120, height / 6), 0, -5, 185, Scalar(0), 2);		//嘴部橢圓邊界
// 	line(image, center, Point(width / 2, height / 2 + height / 4 + 18), Scalar(0), 2);	//人中
// 	//左鬍鬚
// 	line(image, Point(width / 2 - 82, 300), Point(52, 275), Scalar(0), 2);
// 	line(image, Point(width / 2 - 82, 320), Point(62, 312), Scalar(0), 2);
// 	line(image, Point(width / 2 - 82, 340), Point(76, 355), Scalar(0), 2);
// 	//右鬍鬚
// 	line(image, Point(width / 2 + 82, 300), Point(460, 275), Scalar(0), 2);
// 	line(image, Point(width / 2 + 82, 320), Point(450, 312), Scalar(0), 2);
// 	line(image, Point(width / 2 + 82, 340), Point(435, 355), Scalar(0), 2);

// 	//鼻子
// 	circle(image, nose, 24, Scalar(100), -1);
// 	circle(image, Point(width / 2 - 8, height / 2), 6, Scalar(255), -1);// 鼻子反光點
// 	circle(image, center, 201, Scalar(0), 3); //整臉外輪廓

// 	String text = "112C52010";
// 	putText(image, text, Point(width / 2 - 90, 490), FONT_HERSHEY_COMPLEX, 1, Scalar(100), 2, 1);

// 	ofstream rawFile("Doraemon.raw", ios::binary);
// 	if (rawFile.is_open()) {
// 		rawFile.write(reinterpret_cast<char*>(image.data), image.total());
// 		rawFile.close();
// 		cout << "儲存為.raw" << endl;
// 	}
// 	else {
// 		cerr << "儲存失敗" << endl;
// 	}
// 	imshow("Doraemon", image);
// 	imwrite("Doraemon.png", image);
// 	waitKey(0);
// 	destroyAllWindows();
// 	return 0;
// }
