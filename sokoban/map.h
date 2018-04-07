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

	

	
	//��һ��ָ����xsb�ļ��й���map���󲢽��ַ���ʾ��Ϊ����
	map(std::string filename);

	//�����ÿ���̨���
	void printMap();
	//get����
	int ** getMapContent();
	
	int getWidth();

	int getHeight();

	//����ȡ��ͼ���˵�λ��
	void getMenPos(Point * p);

	//����ȡp��d�����ϵ���һ����
	void nextPos(direction d, Point p, Point * next);
	
	//ģ������d����������һ��
	bool step(direction d);

	~map();

};
