#pragma once
#pragma once
#include <string>
#define DEFAULT_USER_NAME "root"
#define DEFAULT_USER_PASSWD "root"
#define DEFAULT_PORT 3306
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_DB_NAME "box_man"

#define LINE   48   //����к���
#define COLUMN 48

typedef struct  _USERINFO
{
	int id;            //�û�id
	std::string username;   //�û���
	std::string passwd;     //����, md5����
	int level_id;      //�ؿ�id
}userinfo;

typedef struct  _LEVELINFO
{
	int id;            //�ؿ�id
	std::string name;   //�ؿ�����
	int map_row;     //��ͼ����
	int map_column;
	std::string map_data;    //��ͼ����
	int next_level_id;       //��һ�ؿ�id
}levelinfo;

bool fetch_user_info(userinfo& user);

bool fetch_level_info(levelinfo& level,  int level_id);

bool transform_map_db2array(levelinfo& level, int map[LINE][COLUMN]);

bool update_user_level(userinfo& user, int next_level_id);