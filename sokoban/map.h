#pragma once

#include"stdafx.h"
#include"Point.h"
namespace cris {
	enum direction {
		up, down, left, right

	};

	class map
	{

	private:

		int mapWidth, mapHeight;
		int **mapContent;

		map(int width, int height);
	/*	void readMapFromFile(std::string fileName);*/

	public:




		//��һ��ָ����xsb�ļ��й���map���󲢽��ַ���ʾ��Ϊ����
		map(std::string filename);

		//�����ÿ���̨���
		void printMap();
		//get����
		void getMapContent(int **,int bufSizeA,int bufSizeB);
		void save(char *filepath, char *savename);
		int getWidth();

		int getHeight();

		//����ȡ��ͼ���˵�λ��
		void getMenPos(Point * p);

		//����ȡp��d�����ϵ���һ����
		void nextPos(direction d, Point p, Point * next);

		//ģ������d����������һ��
		bool step(direction d);
		bool isComplete() 
		{
			for(int i=0;i<mapHeight;i++)
				for (int j = 0; j < mapWidth; j++) 
				{
					if (mapContent[i][j] == 1)
						return false;
				}
			return true;
		}
		bool jump(int i, int j);
		int* operator[](int i)
		{
			return mapContent[i];
		}
		~map();

	};
}