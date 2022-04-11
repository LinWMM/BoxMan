#pragma once

#pragma once

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650

#define RATIO 50

//控制键 上、下、左、右 控制方向，'q' 退出
#define	KEY_UP        'w'    //char  'a'   
#define	KEY_LEFT      'a'
#define	KEY_RIGHT     'd'
#define	KEY_DOWN      's'
#define KEY_QUIT      'q'

//#define LINE   9
//#define COLUMN 12

#define START_X  100
#define START_Y  100

//#define   PROPS    enum _PROPS
typedef   enum _PROPS      PROPS;
typedef   enum _DIRECTION  DIRECTION;
typedef   struct _POS      POS;

#define isValid(pos)  pos.x>=0 && pos.x<LINE && pos.y>=0 && pos.y <COLUMN

enum _PROPS {
	WALL = 0,     //墙
	FLOOR,    //地板
	DES,  //箱子目的地
	MAN,      //小人
	BOX,      //箱子
	MAN_DES,   //人在目的地上
	BOX_DES,    //箱子在目的地上
	ALL
};

//游戏控制方向
enum _DIRECTION
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {
	int x;  //小人所在的二维数组的行
	int y;  //小人所在的二维数组的列
};
