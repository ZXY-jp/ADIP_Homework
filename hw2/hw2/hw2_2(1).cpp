/*為使個檔案不互相干擾，
在main前都有加上_(題號)作區別，
執行前須先將符號刪除*/

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cstring>
using namespace std;
const int ROWS = 10;
const int COLS = 10;
int parent1[ROWS][COLS];
int path1[ROWS][COLS];


//Dm須滿足：1. 有D4走D4   2.沒有D4且DpDq的D8重疊處皆!=V{}
// map[x][y]新的點
// 1. 往右上走：map[x+1][y-1] !=0
//判斷是否Dm可以走以及是否到了邊緣
bool isDmValid(int x, int y, int** map) {
	return x >= 0 && x < ROWS&& y >= 0 && y < COLS&& map[x][y] == 0 && parent1[x][y] == -1;
}

//判斷是否可以走以及是否到了邊緣
bool isValid(int x, int y, int** map) {
	return x >= 0 && x < ROWS&& y >= 0 && y < COLS&& map[x][y] == 0 && parent1[x][y] == -1;
}

void bfs(int** map, int* dx, int* dy, int direction) {
	memset(parent1, -1, sizeof(parent1));
	parent1[0][0] = -2;  // Start node

	//初始化
	int queueX[ROWS * COLS];
	int queueY[ROWS * COLS];
	int front = 0, rear = 1;
	queueX[0] = 0;
	queueY[0] = 0;

	while (front < rear) {
		int x = queueX[front]; //當前位置
		int y = queueY[front];
		front++;

		for (int i = 0; i < direction; i++) {
			int newX = x + dx[i];	//x方向的移動
			int newY = y + dy[i];	//y方向的移動

			if (isValid(newX, newY, map)) { //這條路可行
				parent1[newX][newY] = x * COLS + y; //紀錄現在位置
				queueX[rear] = newX;
				queueY[rear] = newY;
				rear++;
			}
		}
	}
}

void bfs_dm(int** map, int* dx, int* dy, int direction) {
	memset(parent1, -1, sizeof(parent1));
	parent1[0][0] = -2;  // Start node

	//初始化
	int queueX[ROWS * COLS];
	int queueY[ROWS * COLS];
	int front = 0, rear = 1;
	queueX[0] = 0;
	queueY[0] = 0;

	while (front < rear) {
		int x = queueX[front]; //當前位置
		int y = queueY[front];
		front++;

		for (int i = 0; i < direction; i++) {
			int newX = x + dx[i];	//x方向的移動
			int newY = y + dy[i];	//y方向的移動
			if (i < 4) {
				if (isValid(newX, newY, map)) { //這條路可行
					parent1[newX][newY] = x * COLS + y; //紀錄現在位置
					queueX[rear] = newX;
					queueY[rear] = newY;
					rear++;
				}
			}
			else {
				//右下, 左上, 右上, 左下
				//,1,-1,1,-1 
				//,1,-1,-1,1 
				if (isValid(newX, newY, map) && map[max(0,min(x,9))][newY]!=0 && map[newX][max(0,min(y,9))]!=0) { //這條路可行
					parent1[newX][newY] = x * COLS + y; //紀錄現在位置
					queueX[rear] = newX;
					queueY[rear] = newY;
					rear++;
				}
			}
		}
	}
}

void constructPath() {
	int x = ROWS - 1;
	int y = COLS - 1;
	memset(path1, 0, sizeof(path1)); //初始化path

	while (x != 0 || y != 0) {	//回朔路線
		path1[x][y] = 1;  //做紀錄
		int next = parent1[x][y];
		x = next / COLS;
		y = next % COLS;
	}
	path1[0][0] = 1;  // 起始點也包含在路徑上
}

void savePath(int flag) {
	FILE* output_file;

	char  path_D4[] = "2(1)_path_D4.raw";               // Out path
	char  path_D8[] = "2(1)_path_D8.raw";               // Out path
	char  path_Dm[] = "2(1)_path_Dm.raw";               // Out path

	unsigned char* savepath = new unsigned char[10 * 10];
	if (flag == 1) {
		output_file = fopen(path_D4, "wb");
	}
	else if (flag == 2)
	{
		output_file = fopen(path_D8, "wb");
	}
	else {
		output_file = fopen(path_Dm, "wb");
	}
	int step = 0;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (path1[i][j]) {
				savepath[i * 10 + j] = 0;
				//cout << "0 ";
				step++;
			}
			else {
				savepath[i * 10 + j] = 255;
				//cout << "1 ";
			}
		}
		//cout << endl;
	}
	if (flag == 1) {
		cout << "2(1): With D4, the shorest distance from (0,0) to (9,9) is: " << step << endl;
	}
	else if (flag == 2)
	{
		cout << "2(1): With D8, the shorest distance from (0,0) to (9,9) is: " << step << endl;
	}
	else
	{
		cout << "2(1): With Dm, the shorest distance from (0,0) to (9,9) is: " << step << endl;
	}
	fwrite(savepath, 1, 10 * 10, output_file);
	fclose(output_file);
}

void  _21main() {
	char  map[] = "map10x10.raw";                 // Input  raw image name
	unsigned char* arr_map = new unsigned char[ROWS * COLS]; // array for原始10*10資料
	FILE* input_file;

	int** darr_map = new int* [10];
	for (int i = 0; i < 10; i++) {
		darr_map[i] = new int[10];
	}


	input_file = fopen(map, "rb");

	if (input_file == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	fread(arr_map, 1, ROWS * COLS, input_file);
	//建立好二維的地圖

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			//1代表牆壁，0代表可通行
			if (arr_map[i * ROWS + j] == 72) {
				darr_map[i][j] = 0;
			}
			else {
				darr_map[i][j] = 1;
			}
			//cout << darr_map[i][j] << ", ";
		}
		//cout << endl;
	}
	/*----------------------------------------------------------------------------*/
	//定義方向D4(下 上 右 左)
	int dx[] = { 1, -1, 0, 0 };
	int dy[] = { 0, 0, 1, -1 };
	int direct = 4;
	bfs(darr_map, dx, dy, direct);
	constructPath();
	if (parent1[ROWS - 1][COLS - 1] == -1) {
		cout << "No path found." << endl;
	}
	else {
		savePath(1);	//1代表D4
	}

	/*----------------------------------------------------------------------------*/
	//定義方向D8(下, 上, 右, 左, 右下, 左上, 右上 , 左下)
	int dx8[] = { 1, -1, 0, 0 ,1,-1,1,-1 };
	int dy8[] = { 0, 0, 1, -1 ,1,-1,-1,1 };
	direct = 8;
	bfs(darr_map, dx8, dy8, direct);
	constructPath();
	if (parent1[ROWS - 1][COLS - 1] == -1) {
		cout << "No path found." << endl;
	}
	else {
		savePath(2);	//2代表D8
	}

	/*----------------------------------------------------------------------------*/
	//定義方向Dm(下, 上, 右, 左, 右下, 左上, 右上 , 左下)，進去再判斷
	/*int dx8[] = { 1, -1, 0, 0 ,1,-1,1,-1 };
	int dy8[] = { 0, 0, 1, -1 ,1,-1,-1,1 };*/
	direct = 8;
	bfs_dm(darr_map, dx8, dy8, direct);
	constructPath();
	if (parent1[ROWS - 1][COLS - 1] == -1) {
		cout << "No path found." << endl;
	}
	else {
		savePath(3);	//3代表Dm
	}

	delete[] arr_map;

	for (int i = 0; i < 10; i++) {
		delete[] darr_map[i];
	}
	delete[] darr_map;

	fclose(input_file);
}