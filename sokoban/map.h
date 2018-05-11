#pragma once

#include"stdafx.h"
#include"Point.h"
#include"direction.h"
#include"user.h"
namespace cris {
	
	class map
	{

	private:

		int mapWidth, mapHeight;
		int **mapContent;
		map(int width, int height);
		void cleanup();
		bool dfs(Point start,Point end,std::vector<Point> &path,bool **visited) 
		{
			Point p = { 0,0 };
			direction d[] = { up,down,left,right };
			for (int i = 0; i < 4; i++) 
			{
				start.move(p,d[i]);
				//有效点
				if (p.x >= 0 && p.x < mapHeight&&p.y >= 0 && p.y < mapWidth && !visited[p.x][p.y] && mapContent[p.x][p.y]>4) 
				{
					visited[p.x][p.y] = true;
					//是目标点
					if (p.x == end.x&&p.y == end.y)
					{
						path.push_back(p);
						return true;
					}
					else 
					{
						//不是目标点
						path.push_back(p);
						//搜索邻近点是否可到达及路径
						if (!dfs(p, end, path, visited)) {
							path.pop_back();
						}
						else {
							return true;
						}
					}
				}
				
			}
			return false;
		}
		bool dfs(Point start, Point end,std::string &lurd) 
		{
			std::vector<Point> path;
			bool *visited[50];
			for (int i = 0; i < 50; i++) 
			{
				visited[i] = new bool[50];
				for(int j =0;j<50;j++)
				{
					visited[i][j] = false;
				}
			}
			path.push_back(start);
			bool rt = dfs(start, end, path, visited);
			path2lurd(path, lurd);
			for (int i = 0; i < 50; i++) 
			{
				delete[] visited[i];
			}
			return rt;
		}
		void path2lurd(const std::vector<Point> &path,std::string &lurd) 
		{
			Point p;
			for (int i = 0; i < path.size() - 1; i++) 
			{
				direction d[] = { up,left,down,right };
				for (int j = 0; j < 4; j++) 
				{
					path[i].move(p, d[j]);
					if (p == path[i + 1]) 
					{
						switch (j) 
						{
						case 0:
							lurd += 'u';
							break;

						case 1:
							lurd += 'l';
							break;

						case 2:
							lurd += 'd';
							break;

						case 3:
							lurd += 'r';
							break;

						}
					}
				}
			}
		}
	public:
		std::string mappath;



		void readMapFromFile(std::string fileName);

		//从一个指定的xsb文件中构造map对象并将字符表示改为数字
		map(std::string filename);

		//测试用控制台输出
		void printMap();
		//get方法
		void getMapContent(int **,int bufSizeA,int bufSizeB);
		void save(char *filepath, char *savename);
		int getWidth();

		int getHeight();

		//用于取地图中人的位置
		void getMenPos(Point * p);

		//用于取p在d方向上的下一个点
		void nextPos(direction d, Point p, Point * next);

		//模拟人在d方向上行走一步
		bool step(direction d);
		void back(user&u);
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
		bool jump(int i, int j,user &u);
		int* operator[](int i)
		{
			return mapContent[i];
		}
		~map();

	};
}