#include"map.h"
map::map(int width, int height)
{
	mapWidth = width;
	mapHeight = height;
	mapContent = new int*[height];
	for (int i = 0; i < height; i++)
	{
		mapContent[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			mapContent[i][j] = 4;
		}
	}
}

map::map(std::string fileName)
{
	int width = 0;
	int height = 0;
	int lineNum = 0;
	std::string line;
	bool f = false;
	std::fstream file;
	file.open(fileName, std::ios::in);
	while (!file.eof())
	{
		height++;

		std::getline(file, line);
		if (line.length() > width)
			width = line.length();
	}
	std::cout << height << '-' << width << std::endl;

	mapWidth = width;
	mapHeight = height;
	mapContent = new int*[height];
	for (int i = 0; i < height; i++)
	{
		mapContent[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			mapContent[i][j] = 4;
		}
	}	int **p = mapContent;

	file.clear();
	file.seekg(std::ios::beg);
	while (!file.eof()) {
		std::getline(file, line);

		for (int i = 0; i < 50; i++)
		{
			switch (line[i])
			{
				//box
			case '$':
				p[lineNum][i] = 1;
				break;
				//human
			case '@':
				p[lineNum][i] = 2;
				break;

			case '*':
				p[lineNum][i] = 3;
				break;

			case '#':
				p[lineNum][i] = 4;
				break;

			case '.':
				p[lineNum][i] = 5;
				break;

			case '-':
			case '_':
			case ' ':
				p[lineNum][i] = 6;
				break;
			case '+':
				p[lineNum][i] = 7;;
				break;

			default:
				f = true;
			}
			if (f)
			{
				f = false;
				break;
			}
		}
		lineNum++;
	}

}

void map::readMapFromFile(std::string fileName)
{
	/* 	@ == > 人 (man)
	+ == > 人在目标点(man on goal)
	$ == > 箱子(box)
	* == > 箱子在目标点(box on goal)
	# == > 墙 (wall)
	. == > 目标点(goal)
	- == > XSB格式空格代表“地板”，又因为连续多个空格在网页或即时通讯软件中偶尔显示有问题，
	也用“ - ”或“_”代替空格。(floor, represented by ' ' or '-' or '_')
	*/
	std::string line;
	int **p = mapContent;
	int lineNum = 0;
	bool f = false;
	std::fstream file;
	file.open(fileName, std::ios::in);
	while (!file.eof()) {
		std::getline(file, line);

		for (int i = 0; i < 50; i++)
		{
			switch (line[i])
			{
				//箱子
			case '$':
				p[lineNum][i] = 1;
				break;
				//人
			case '@':
				p[lineNum][i] = 2;
				break;
				//箱子在目标点
			case '*':
				p[lineNum][i] = 3;
				break;
				//墙
			case '#':
				p[lineNum][i] = 4;
				break;
				//目标点
			case '.':
				p[lineNum][i] = 5;
				break;
				//地板
			case '-':
			case '_':
			case ' ':
				p[lineNum][i] = 6;
				break;


				//+  7 人再目标点
			case '+':
				p[lineNum][i] = 7;;
				break;

			default:
				f = true;
			}
			if (f)
			{
				f = false;
				break;
			}
		}
		lineNum++;
	}
}

void map::printMap()
{
	int **p = mapContent;
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			switch (mapContent[i][j])
			{
				//箱子
			case 1:
				std::cout << '$';
				break;
				//人
			case 2:
				std::cout << '@';
				break;
				//箱子在目标点
			case 3:
				std::cout << '*';
				break;
				//墙
			case 4:
				std::cout << '#';
				break;
				//目标点
			case 5:
				std::cout << '.';
				break;
				//地板
			case 6:
				std::cout << ' ';
				break;
			case 7:
				std::cout << '+';
				break;


				//+  7 人再目标点

			}
		}
		std::cout << std::endl;
	}
}
int ** map::getMapContent()
{
	return mapContent;
}
int map::getWidth()
{
	return mapWidth;
}
int map::getHeight()
{
	return mapHeight;
}
void map::getMenPos(Point*p)
{
	bool b = false;
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapContent[i][j] == 2)
			{
				b = true;
				p->x = i;
				p->y = j;
				return;
			}
		}
	}
	int k = 0;
	if (!b)
		for (int i = 0; i < mapHeight; i++)
		{
			for (int j = 0; j < mapWidth; j++)
			{
				if ((k=mapContent[i][j]) == 7)
				{
					b = true;
					p->x = i;
					p->y = j;
					return;
				}
			}
		}

}
void map::nextPos(direction d, Point p, Point *next) {
	
	switch (d) {
	case up:
		p.up(next);
		break;
	case down:
		p.down(next);
		break;
	case left:
		p.left(next);
		break;

	case right:
		p.right(next);
		break;
	}



}
bool map::step(direction d)
{
	Point men, n1, next2;

	getMenPos(&men);

	nextPos(d, men,&n1);
	nextPos(d, n1, &next2);
	std::cout << n1.x;
	std::cout << n1.y;
	std::cout << next2.x;
	std::cout << next2.y;
	switch (mapContent[men.x][men.y])
	{
	case 2:
		switch (mapContent[n1.x][n1.y])
		{
		case 6:
			mapContent[men.x][men.y] = 6;
			mapContent[n1.x][n1.y] = 2;
			return true;
		case 5:
			mapContent[men.x][men.y] = 6;
			mapContent[n1.x][n1.y] = 7;
			return true;
		case 4:
			return false;
		case 1:
			switch (mapContent[next2.x][next2.y])
			{
			case 6:
				mapContent[men.x][men.y] = 6;
				mapContent[n1.x][n1.y] = 2;
				mapContent[next2.x][next2.y] = 1;
				return true;

			case 5:
				mapContent[men.x][men.y] = 6;
				mapContent[n1.x][n1.y] = 2;
				mapContent[next2.x][next2.y] = 3;
				return true;

			case 4:
			case 1:
			case 3:
				return false;
			}
		case 3:
			switch (mapContent[next2.x][next2.y])
			{
			case 6:
				mapContent[men.x][men.y] = 6;
				mapContent[n1.x][n1.y] = 7;
				mapContent[next2.x][next2.y] = 1;
				return true;

			case 5:
				mapContent[men.x][men.y] = 6;
				mapContent[n1.x][n1.y] = 7;
				mapContent[next2.x][next2.y] = 3;
				return true;

			case 4:
			case 1:
			case 3:
				return false;
			}
		}
	case 7:

			switch (mapContent[n1.x][n1.y])
			{
			case 6:
				mapContent[men.x][men.y] = 5;
				mapContent[n1.x][n1.y] = 2;
				return true;
			case 5:
				mapContent[men.x][men.y] = 5;
				mapContent[n1.x][n1.y] = 7;
				return true;
			case 4:
				return false;
			case 1:
				switch (mapContent[next2.x][next2.y])
				{
				case 6:
					mapContent[men.x][men.y] = 5;
					mapContent[n1.x][n1.y] = 2;
					mapContent[next2.x][next2.y] = 1;
					return true;

				case 5:
					mapContent[men.x][men.y] = 5;
					mapContent[n1.x][n1.y] = 2;
					mapContent[next2.x][next2.y] = 3;
					return true;

				case 4:
				case 1:
				case 3:
					return false;
				}
			case 3:
				switch (mapContent[next2.x][next2.y])
				{
				case 6:
					mapContent[men.x][men.y] = 5;
					mapContent[n1.x][n1.y] = 7;
					mapContent[next2.x][next2.y] = 1;
					return true;

				case 5:
					mapContent[men.x][men.y] = 5;
					mapContent[n1.x][n1.y] = 7;
					mapContent[next2.x][next2.y] = 3;
					return true;

				case 4:
				case 1:
				case 3:
					return false;
				}
			}

		

	}
	return false;
}
map::~map()
{
	for (int i = 0; i < mapHeight; i++)
	{
		delete mapContent[i];
	}
	delete mapContent;
}