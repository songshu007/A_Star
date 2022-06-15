#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <graphics.h>
#include <time.h>
#include <Windows.h>
#include <algorithm>

using namespace std;

#define WIDTH 1080 //分辨率
#define HEIGHT 720
#define BLOCK_XY 20	//小方格边长
const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);	 //小方格x轴数量
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY); //小方格y轴数量

#define kongqi COLORREF RGB(200,200,200)//空气颜色
#define qiangbi COLORREF RGB(0,0,0)//墙壁颜色
#define zhongdian COLORREF RGB(0,0,255)//终点颜色
#define qidian COLORREF RGB(255,0,0)//起点颜色
#define openblock COLORREF RGB(0,255,0)//待搜索方块颜色
#define closeblock COLORREF RGB(100,100,100)//已搜索方块颜色
#define lu COLORREF RGB(255,100,100)//路颜色

class block {
public:
	int x;
	int y;
	int f; //当前(路程)代价
	int g; //预估代价
	int h; //h = f + g
	int move = 0;  //0:空气 / 1:墙壁 / 2:起点 / 3:终点 / 4:openlist / 5:closelist / 6:路
	block* father; //父节点
	COLORREF color = kongqi;

	block() {
	
	};

	block(int x, int y, int move) {
		this->x = x;
		this->y = y;
		this->move = move;
		f = 0;
		g = 0;
		h = 0;
		father = NULL;
	}
};

block screen[BLOCK_HEIGHT][BLOCK_WIDTH];

vector<block*> openlist;  //开放列表
vector<block*> closelist;  //关闭列表

block* starblock = new block(0,0,2); //起始点
block* endblock = new block(10,20,3); //终点

void display();
void init();
bool cmd(block* n1, block* n2);
void star();
void checkmove(int x, int y, block* father);
void drawroad(block* root);
void gamedrawpixal(int x, int y, COLORREF COLOR);


int main() {
	initgraph(WIDTH, HEIGHT);//创建绘图窗口
	BeginBatchDraw();//开始批量绘图
	cleardevice();//清空显示
	
	init();
	star();

	while (1);
}

bool cmd(block* n1, block* n2) {
	return n1->h < n2->h;
}

void star() {
	int _find = 0;
	block* root = NULL;
	while (openlist.size() > 0) {
		root = openlist[0];
		
		if (root->x == endblock->x && root->y == endblock->y) {
			_find = 1;
			break;
		}

		//上
		if (root->y > 0) {
			checkmove(root->x, root->y - 1, root);
		}
		//下
		if (root->y < BLOCK_HEIGHT - 1) {
			checkmove(root->x, root->y + 1, root);
		}
		//左
		if (root->x > 0) {
			checkmove(root->x - 1, root->y, root);
		}
		//右
		if (root->x < BLOCK_WIDTH - 1) {
			checkmove(root->x + 1, root->y, root);
		}

		if (_find) {
			break;
		}

		closelist.push_back(root);
		gamedrawpixal(root->x, root->y, closeblock);
		if (!(root->x == starblock->x && root->y == starblock->y)) {
			screen[root->y][root->x].move = 5;
		}
		openlist.erase(openlist.begin());
		sort(openlist.begin(), openlist.end(), cmd);

		display();
		FlushBatchDraw();//将缓冲输出到屏幕
	}
	if (_find) {
		drawroad(root);
	}

}

void drawroad(block* root) {
	setfillcolor(lu); setlinecolor(lu);
	while(root->father != NULL){
		gamedrawpixal(root->father->x, root->father->y, lu);
		root->father = root->father->father;
		Sleep(10);
		FlushBatchDraw();//将缓冲输出到屏幕
	}
}

void checkmove(int x, int y, block* father) {
	if (screen[y][x].move == 5) {
		return;
	}
	if (screen[y][x].move == 1) {
		return;
	}
	if (screen[y][x].move == 3) {
		openlist.push_back(&screen[y][x]);//将这个点加入openlist
		screen[y][x].father = father;
		return;
	}
	if (screen[y][x].move == 0) {
		screen[y][x].father = father;
		screen[y][x].move = 4;
		screen[y][x].f = father->f + 1;
		screen[y][x].g = abs(screen[y][x].x - endblock->x) + abs(screen[y][x].y - endblock->y);
		//screen[y][x].g = sqrt(pow(screen[y][x].x - endblock->x,2) + pow(screen[y][x].y - endblock->y,2));
		screen[y][x].h = screen[y][x].f + screen[y][x].g;
		openlist.push_back(&screen[y][x]);//将这个点加入openlist
		gamedrawpixal(screen[y][x].x,screen[y][x].y,openblock);
	}
	if (screen[y][x].move == 4) {
		/*
		if (screen[y][x].h < father->h) {
			screen[y][x].father = father;
			screen[y][x].move = 4;

			//screen[y][x].f = father->f + 1;
			screen[y][x].g = abs(screen[y][x].x - endblock->x) + abs(screen[y][x].y - endblock->y);
			//screen[y][x].g = sqrt(pow(screen[y][x].x - endblock->x, 2) + pow(screen[y][x].y - endblock->y, 2));
			screen[y][x].h = screen[y][x].f + screen[y][x].g;
			
			gamedrawpixal(screen[y][x].x, screen[y][x].y, openblock);
		}*/
	}

}

void init(){
	setbkcolor(kongqi);
	for (int i = 0; i < BLOCK_HEIGHT; i++){
		for (int j = 0; j < BLOCK_WIDTH; j++) {
			screen[i][j].x = j;
			screen[i][j].y = i;
		}
	}
	
	for (int i = 0; i < 30; i++){
		screen[i][5].move = 1;
	}

	for (int i = 3; i < 40; i++) {
		screen[i][8].move = 1;
	}

	for (int i = 8; i < 45; i++) {
		screen[3][i].move = 1;
	}

	for (int i = 3; i < 30; i++) {
		screen[i][45].move = 1;
	}

	for (int i = 10; i < 30; i++) {
		screen[i][15].move = 1;
	}

	for (int i = 10; i < 40; i++) {
		screen[i][35].move = 1;
	}

	for (int i = 15; i < 36; i++) {
		screen[30][i].move = 1;
	}

	for (int i = 15; i < 36; i++) {
		screen[10][i].move = 1;
	}
	
	screen[starblock->y][starblock->x].move = starblock->move;
	screen[endblock->y][endblock->x].move = endblock->move;

	openlist.push_back(starblock);//将起点放入openlist

}

void display() {
	for (int j = 0; j < BLOCK_HEIGHT; j++) {
		for (int i = 0; i < BLOCK_WIDTH; i++) {//0:空气 / 1:墙壁 / 2:起点 / 3:终点 / 4:openlist / 5:closelist / 6:路

			switch (screen[j][i].move){
			case 0:
				setfillcolor(kongqi); setlinecolor(kongqi);
				break;
			case 1:
				setfillcolor(qiangbi); setlinecolor(qiangbi);
				break;
			case 2:
				setfillcolor(qidian); setlinecolor(qidian);
				break;
			case 3:
				setfillcolor(zhongdian); setlinecolor(zhongdian);
				break;
			case 4:
				setfillcolor(openblock); setlinecolor(openblock);
				break;
			case 5:
				setfillcolor(closeblock); setlinecolor(closeblock);
				break;
			case 6:
				setfillcolor(lu); setlinecolor(lu);
				break;
			default:
				break;
			}
			fillrectangle(i * BLOCK_XY, j * BLOCK_XY, BLOCK_XY + i * BLOCK_XY, BLOCK_XY + j * BLOCK_XY);

		}
	}
	FlushBatchDraw();//将缓冲输出到屏幕
}

void gamedrawpixal(int x, int y, COLORREF COLOR) {
	setfillcolor(COLOR); setlinecolor(COLORREF RGB(0,0,0));
	fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
};

