#pragma once
#pragma once
#include <string>
#define DEFAULT_USER_NAME "root"
#define DEFAULT_USER_PASSWD "root"
#define DEFAULT_PORT 3306
#define DEFAULT_HOST "127.0.0.1"
#define DEFAULT_DB_NAME "box_man"

#define LINE   48   //最大行和列
#define COLUMN 48

typedef struct  _USERINFO
{
	int id;            //用户id
	std::string username;   //用户名
	std::string passwd;     //密码, md5加密
	int level_id;      //关卡id
}userinfo;

typedef struct  _LEVELINFO
{
	int id;            //关卡id
	std::string name;   //关卡名字
	int map_row;     //地图行数
	int map_column;
	std::string map_data;    //地图数据
	int next_level_id;       //下一关卡id
}levelinfo;

bool fetch_user_info(userinfo& user);

bool fetch_level_info(levelinfo& level,  int level_id);

bool transform_map_db2array(levelinfo& level, int map[LINE][COLUMN]);

bool update_user_level(userinfo& user, int next_level_id);