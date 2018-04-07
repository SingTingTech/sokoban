#pragma once

#include"stdafx.h"
#include"Point.h"
enum direction {
	up,down,left,right

};

class map
{

private:

	int mapWidth, mapHeight;
	int **mapContent;

	map(int width, int height);
	void readMapFromFile(std::string fileName);

public:

	

	
	//从一个指定的xsb文件中构造map对象并将字符表示改为数字
	map(std::string filename);

	//测试用控制台输出
	void printMap();
	//get方法
	int ** getMapContent();
	
	int getWidth();

	int getHeight();

	//用于取地图中人的位置
	void getMenPos(Point * p);

	//用于取p在d方向上的下一个点
	void nextPos(direction d, Point p, Point * next);
	
	//模拟人在d方向上行走一步
	bool step(direction d);

	~map();

};
