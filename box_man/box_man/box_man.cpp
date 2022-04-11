#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include "database.h"
#include "box_man.h"

using namespace std;
#define IMPROVE

IMAGE images[ALL];
IMAGE bg_img;

struct _POS man;  //小人在二维数组中的位置

/*游戏地图*/
int map[LINE][COLUMN] = { 0 };
/*int map[LINE][COLUMN] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 1, 4, 1, 0, 2, 1, 0, 2, 1, 0, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0 },
	{ 0, 1, 0, 2, 0, 1, 1, 4, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 0, 3, 1, 1, 1, 4, 1, 0 },
	{ 0, 1, 2, 1, 1, 4, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};*/



//*判断游戏是否结束，如果不存在任何一个箱子目的地，就代表游戏结束
bool isGameOver() {
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (map[i][j] == DES) return false;
		}
	}
	return true;
}

//提示玩家游戏结束，游戏结束场景，在玩家通关后显示
void gameOverScene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, _T("宋体"));
	drawtext(_T("恭喜您~\n已经成为了一个合格的推箱子老司机！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	::system("pause");
}

//提示玩家进入下一关
void gameNextScene(IMAGE* bg) {
	putimage(0, 0, bg);
	settextcolor(WHITE);
	RECT rec = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	settextstyle(20, 0, _T("宋体"));
	drawtext(_T("恭喜您~\n任意键跳转下一关！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	::system("pause");
	cleardevice();
}


//刷新地图，MAN每走一步，刷新一次
void changeMap(POS* pos, PROPS prop) {
	map[pos->x][pos->y] = prop;
	putimage(START_X + pos->y * RATIO, START_Y + pos->x * RATIO, &images[prop]);
}

//方向控制
void gameControl(DIRECTION direct) {

	POS next_pos = man;
	POS next_next_pos = man;
	switch (direct) {
	case UP:
		next_pos.x--;
		next_next_pos.x -= 2;
		break;
	case DOWN:
		next_pos.x++;
		next_next_pos.x += 2;
		break;
	case LEFT:
		next_pos.y--;
		next_next_pos.y -= 2;
		break;
	case RIGHT:
		next_pos.y++;
		next_next_pos.y += 2;
		break;
	}
#ifndef IMPROVE
	//宏展开 next_pos.x>=0 && next_pos.x<LINE && next_pos.y>=0 && next_pos.y <COLUMN
	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR)) {//人的前方是地板
		changeMap(&next_pos, MAN); //小人前进一格
		changeMap(&man, FLOOR);
		man = next_pos;
	}
	else if (isValid(next_next_pos) && map[next_pos.x][next_pos.y] == BOX)
	{//人的前方是箱子
		if (map[next_next_pos.x][next_next_pos.y] == FLOOR) {
			changeMap(&next_next_pos, BOX);
			changeMap(&next_pos, MAN); //小人前进一格
			changeMap(&man, FLOOR);
			man = next_pos;
		}
		else if (map[next_next_pos.x][next_next_pos.y] == DES) {
			changeMap(&next_next_pos, BOX_DES);
			changeMap(&next_pos, MAN); //小人前进一格
			changeMap(&man, FLOOR);
			man = next_pos;
		}
	}
#else
	if (isValid(next_pos) && (map[next_pos.x][next_pos.y] == FLOOR || map[next_pos.x][next_pos.y] == DES)) {
		if (map[next_pos.x][next_pos.y] == FLOOR)
		{
			if (map[man.x][man.y] == MAN)
			{
				changeMap(&next_pos, MAN);
				changeMap(&man, FLOOR);
			}
			else if (map[man.x][man.y] == MAN_DES)//人当前在目的地上
			{
				changeMap(&next_pos, MAN);
				changeMap(&man, DES);
			}
		}
		else if (map[next_pos.x][next_pos.y] == DES)
		{
			if (map[man.x][man.y] == MAN)
			{
				changeMap(&next_pos, MAN_DES);
				changeMap(&man, FLOOR);
			}
			else if (map[man.x][man.y] == MAN_DES)
			{
				changeMap(&next_pos, MAN_DES);
				changeMap(&man, DES);
			}
		}
		man = next_pos;
	}

	//下一个是箱子
	else if (isValid(next_next_pos) && (map[next_pos.x][next_pos.y] == BOX || map[next_pos.x][next_pos.y] == BOX_DES))
	{
		if (map[next_pos.x][next_pos.y] == BOX)
		{
			if (map[next_next_pos.x][next_next_pos.y] == FLOOR)
			{
				changeMap(&next_next_pos, BOX);
				changeMap(&next_pos, MAN);
				if (map[man.x][man.y] == MAN)
					changeMap(&man, FLOOR);
				else if (map[man.x][man.y] == MAN_DES)
					changeMap(&man, DES);
				man = next_pos;
			}
			else if (map[next_next_pos.x][next_next_pos.y] == DES)
			{
				changeMap(&next_next_pos, BOX_DES);
				changeMap(&next_pos, MAN);
				if (map[man.x][man.y] == MAN)
					changeMap(&man, FLOOR);
				else if (map[man.x][man.y] == MAN_DES)
					changeMap(&man, DES);
				man = next_pos;
			}
		}
		else if (map[next_pos.x][next_pos.y] == BOX_DES)
		{
			if (map[next_next_pos.x][next_next_pos.y] == FLOOR)
			{
				changeMap(&next_next_pos, BOX);
				changeMap(&next_pos, MAN_DES);
				if (map[man.x][man.y] == MAN)
					changeMap(&man, FLOOR);
				else if (map[man.x][man.y] == MAN_DES)
					changeMap(&man, DES);
				man = next_pos;
			}
			else if (map[next_next_pos.x][next_next_pos.y] == DES)
			{
				changeMap(&next_next_pos, BOX_DES);
				changeMap(&next_pos, MAN_DES);
				if (map[man.x][man.y] == MAN)
					changeMap(&man, FLOOR);
				else if (map[man.x][man.y] == MAN_DES)
					changeMap(&man, DES);
				man = next_pos;
			}
		}
	}
#endif 

}

//用户登录
bool login(userinfo& user) {
	int times = 0;
	bool ret = false;

#define MAX_RETRY_TIMES  4    //最大输入次数
	do {
		cout << "请输入用户名: ";
		cin >> user.username;

		cout << "请输入密码: ";
		cin >> user.passwd;

		//返回 bool ,成功返回true ,失败返回false .
		ret = fetch_user_info(user);
		times++;

		if (times >= MAX_RETRY_TIMES) {
			break;
		}
		if (ret == false) {
			cout << "登陆失败，请重新输入!" << endl;
		}
	} while (!ret);

	return ret;
}

//初始化背景
void initGameGraphic()
{
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	loadimage(&bg_img, _T("blackground.bmp"), SCREEN_WIDTH, SCREEN_HEIGHT, true);
	putimage(0, 0, &bg_img);

	//加载道具图标
	loadimage(&images[WALL], _T("picture/wall_right.bmp"), RATIO, RATIO, true);
	loadimage(&images[FLOOR], _T("picture/floor.bmp"), RATIO, RATIO, true);
	loadimage(&images[DES], _T("picture/des.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN], _T("picture/man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX], _T("picture/box.bmp"), RATIO, RATIO, true);
	loadimage(&images[MAN_DES], _T("picture/man.bmp"), RATIO, RATIO, true);
	loadimage(&images[BOX_DES], _T("picture/box.bmp"), RATIO, RATIO, true);
}

int main(void) {
	userinfo user;
	levelinfo level;

	int ret = false;
	/*user.username = "lin";     
	user.passwd = "wanglin";
	*/

	if (!login(user))//
	{
		cout << "login back in" << endl;
		return -1;
	}
	else
	{
		cout << "login correct!" << endl;

	}

	//初始化，绑定图片信息
	initGameGraphic();

	bool quit = false;
	do {
		if (fetch_level_info(level, user.level_id) == false)    //获取关卡信息
		{
			cout << "no such level.\n";
			return -1;
		}
		else
		{
			cout << "load successfully.\n";
			cout << "userid:" << user.id << ", level_id:" << level.id << endl;
		}


		//数据库中的地图数据转换成map
		ret = transform_map_db2array(level, map);

		for (int i = 0; i < level.map_row; i++) {
			for (int j = 0; j < level.map_column; j++) {
				if (map[i][j] == MAN) {
					man.x = i;
					man.y = j;
				}
				putimage(START_X + j * RATIO, START_Y + i * RATIO, &images[map[i][j]]);
			}
		}

		//游戏环节
		do {
			if (_kbhit()) { //玩家按键
				char ch = _getch();

				if (ch == KEY_UP) {
					gameControl(UP);
				}
				else if (ch == KEY_DOWN) {
					gameControl(DOWN);
				}
				else if (ch == KEY_LEFT) {
					gameControl(LEFT);
				}
				else if (ch == KEY_RIGHT) {
					gameControl(RIGHT);
				}
				else if (ch == KEY_QUIT) {
					quit = true;
				}
				if (isGameOver()) {
					if (level.next_level_id < 1)
					{
						gameOverScene(&bg_img);
						quit = true;
						break;
					}
					
					//更新用户信息
					gameNextScene(&bg_img);
					update_user_level(user, level.next_level_id);
					user.level_id = level.next_level_id;
					break;   //调出当前循环，重新加载地图
					//quit = true;
				}
			}
			Sleep(100);
		} while (quit == false); //!quit

	} while (quit == false);

	::system("pause");

	//游戏结束，释放资源
	closegraph();
	::system("pause");
	return 0;
}