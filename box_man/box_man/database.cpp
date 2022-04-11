#include "database.h"
#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

static bool connect_db(MYSQL& mysql)
{
	//初始化MYSQL句柄
	mysql_init(&mysql);

	//设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");    //"gbk"字符编码支持中文

	if (mysql_real_connect(&mysql, DEFAULT_HOST, DEFAULT_USER_NAME, \
		DEFAULT_USER_PASSWD, DEFAULT_DB_NAME, DEFAULT_PORT, NULL, 0) == NULL)
	{
		cout << "connect database:box_man failed" << endl;
		return false;
	}
	return true;
}

bool fetch_user_info(userinfo& user)
{

	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];   //储存要执行的sql语句

	//连接数据库
	if (connect_db(mysql) == false)
	{
		return false;
	}

	snprintf(sql, 256, "select id, level_id from users where username='%s' and password=md5('%s');", \
		user.username.c_str(), user.passwd.c_str());

	if (mysql_query(&mysql, sql) != 0)
	{
		cout << "query failed: " << mysql_error(&mysql) << endl;
		mysql_close(&mysql);
		return false;
	}

	//获取结果
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL)
	{
		cout << "no such result.\n";
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id = atoi(row[1]);
	cout << "user_id: " << user.id << endl;
	cout << "level_id: " << user.level_id << endl;

	mysql_free_result(res);
	mysql_close(&mysql);

	return true;
}

bool fetch_level_info(levelinfo& level, int level_id)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char sql[256];   

	if (connect_db(mysql) == false)
	{
		return false;
	}

	snprintf(sql, 256, "select name, map_row, map_column, map_data, next_level_id \
							from levels where id = %d", level_id);

	if (mysql_query(&mysql, sql) != 0)
	{
		cout << "query failed: " << mysql_error(&mysql) << endl;
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if (row == NULL)
	{
		cout << "no such result.\n";
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	level.id = level_id;
	level.name = row[0];
	level.map_row = atoi(row[1]);
	level.map_column = atoi(row[2]);
	level.map_data = row[3];
	level.next_level_id = atoi(row[4]);

	mysql_free_result(res);
	mysql_close(&mysql);
	return true;
}

bool transform_map_db2array(levelinfo& level, int map[LINE][COLUMN])
{

	if (level.map_row > LINE || level.map_column > COLUMN) {
		printf("map is too large!\n");
		return false;
	}

	if (level.map_data.length() < 1) {
		printf("map_data is too short!\n");
		return false;
	}

	int start = 0, end = 0;
	int row = 0, column = 0;

	do {
		end = level.map_data.find('|', start);

		if (end < 0) {
			end = level.map_data.length();
		}

		if (start >= end) break;     //防止最后多加了一个 |

		string line = level.map_data.substr(start, end - start);    //得到一行数据
		printf("get line: %s\n", line.c_str());

		//对行地图数据进行解析
		char* next_token = NULL;
		char* item = strtok_s((char*)line.c_str(), ",", &next_token);   
		//按照逗号分隔符劈开字符串，把逗号定界符改成'\0'，然后剩下的字符串存在&next_token中

		column = 0;

		while (item && column < level.map_column) {
			printf("%s ", item);
			map[row][column] = atoi(item);
			column++;

			item = strtok_s(NULL, ",", &next_token);
		}

		if (column < level.map_column) {
			printf("parse column failed!\n");
			return false;
		}

		printf("\n");
		row++;

		if (row >= level.map_row) {
			break;
		}

		start = end + 1;

	} while (1);

	::system("pause");

	if (row < level.map_row) {
		printf("parse row failed!\n");
		return false;
	}

	return true;
}

bool update_user_level(userinfo& user, int next_level_id)
{
	MYSQL mysql;
	char sql[256];   //储存要执行的sql语句

	//连接数据库
	if (connect_db(mysql) == false)
	{
		return false;
	}

	snprintf(sql, 256, "update users set level_id = %d where id = %d", \
		next_level_id, user.id);

	int ret;
	ret = mysql_query(&mysql, sql);
	mysql_close(&mysql);
	if (ret == 0)
	{
		return true;
	}
	cout << "update failed.\n";
	return  false;
}
