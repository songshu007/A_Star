#include <vector>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <graphics.h>
#include <time.h>
#include <Windows.h>
#include <algorithm>

using namespace std;

#define WIDTH 1080 //�ֱ���
#define HEIGHT 720
#define BLOCK_XY 20	//С����߳�
const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);	 //С����x������
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY); //С����y������

#define kongqi COLORREF RGB(200,200,200)//������ɫ
#define qiangbi COLORREF RGB(0,0,0)//ǽ����ɫ
#define zhongdian COLORREF RGB(0,0,255)//�յ���ɫ
#define qidian COLORREF RGB(255,0,0)//�����ɫ
#define openblock COLORREF RGB(0,255,0)//������������ɫ
#define closeblock COLORREF RGB(100,100,100)//������������ɫ
#define lu COLORREF RGB(255,100,100)//·��ɫ

class block {
public:
	int x;
	int y;
	int f; //��ǰ(·��)����
	int g; //Ԥ������
	int h; //h = f + g
	int move = 0;  //0:���� / 1:ǽ�� / 2:��� / 3:�յ� / 4:openlist / 5:closelist / 6:·
	block* father; //���ڵ�
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

vector<block*> openlist;  //�����б�
vector<block*> closelist;  //�ر��б�

block* starblock = new block(0,0,2); //��ʼ��
block* endblock = new block(10,20,3); //�յ�

void display();
void init();
bool cmd(block* n1, block* n2);
void star();
void checkmove(int x, int y, block* father);
void drawroad(block* root);
void gamedrawpixal(int x, int y, COLORREF COLOR);


int main() {
	initgraph(WIDTH, HEIGHT);//������ͼ����
	BeginBatchDraw();//��ʼ������ͼ
	cleardevice();//�����ʾ
	
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

		//��
		if (root->y > 0) {
			checkmove(root->x, root->y - 1, root);
		}
		//��
		if (root->y < BLOCK_HEIGHT - 1) {
			checkmove(root->x, root->y + 1, root);
		}
		//��
		if (root->x > 0) {
			checkmove(root->x - 1, root->y, root);
		}
		//��
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
		FlushBatchDraw();//�������������Ļ
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
		FlushBatchDraw();//�������������Ļ
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
		openlist.push_back(&screen[y][x]);//����������openlist
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
		openlist.push_back(&screen[y][x]);//����������openlist
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

	openlist.push_back(starblock);//��������openlist

}

void display() {
	for (int j = 0; j < BLOCK_HEIGHT; j++) {
		for (int i = 0; i < BLOCK_WIDTH; i++) {//0:���� / 1:ǽ�� / 2:��� / 3:�յ� / 4:openlist / 5:closelist / 6:·

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
	FlushBatchDraw();//�������������Ļ
}

void gamedrawpixal(int x, int y, COLORREF COLOR) {
	setfillcolor(COLOR); setlinecolor(COLORREF RGB(0,0,0));
	fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
};

