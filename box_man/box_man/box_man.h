#pragma once

#pragma once

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 650

#define RATIO 50

//���Ƽ� �ϡ��¡����� ���Ʒ���'q' �˳�
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
	WALL = 0,     //ǽ
	FLOOR,    //�ذ�
	DES,  //����Ŀ�ĵ�
	MAN,      //С��
	BOX,      //����
	MAN_DES,   //����Ŀ�ĵ���
	BOX_DES,    //������Ŀ�ĵ���
	ALL
};

//��Ϸ���Ʒ���
enum _DIRECTION
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

struct _POS {
	int x;  //С�����ڵĶ�ά�������
	int y;  //С�����ڵĶ�ά�������
};