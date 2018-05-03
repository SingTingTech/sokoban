#include"map.h"
cris::map::map(int width, int height)
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

cris::map::map(std::string fileName)
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
	height -= 3;
	mapWidth = width;
	mapHeight = height;
	mapContent = new int*[height];
	for (int i = 0; i < height; i++)
	{
		mapContent[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			mapContent[i][j] = 6;
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
	file.clear();
	file.close();
}

void cris::map::readMapFromFile(std::string fileName)
{
	cleanup();
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
	height -= 3;
	mapWidth = width;
	mapHeight = height;
	mapContent = new int*[height];
	for (int i = 0; i < height; i++)
	{
		mapContent[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			mapContent[i][j] = 6;
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
	file.clear();
	file.close();
}

void cris::map::cleanup()
{
	for (int i = 0; i < mapHeight; i++)
	{
		delete mapContent[i];
	}
	delete mapContent;
}

void cris::map::printMap()
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

void cris::map::getMapContent(int **buf, int bufSizeA, int bufSizeB)
{
	if (bufSizeA < getHeight() || bufSizeB < getWidth())
	{
		std::cerr << "过小的buffer";
		return;
	}
	else 
	{
		for (int i = 0; i < mapHeight; i++) 
		{
			for (int j = 0; j < mapWidth; i++) 
			{
				buf[i][j] = mapContent[i][j];
			}
		}
	}
}

void cris::map::save(char * filepath, char * savename)
{
	std::ofstream fout;
	char filename[1024] = { 0 };
	strcpy_s(filename,1024, filepath);
	int strscount = strlen(filename);
	strcpy_s(strscount + filename,1024- strscount, savename);
	fout.open(filename,std::ios::_Noreplace);
	for (int i = 0; i < mapHeight; i++) 
	{
		for (int j = 0; j < mapWidth; j++) 
		{
			switch (mapContent[i][j])
			{
				//box
			case 1:
				fout<<'$';
				break;
				//human
			case 2:
				fout<<'@';
				break;

			case 3:
				fout<<'*';
				break;

			case 4:
				fout<<'#';
				break;

			case 5:
				fout<<'.';
				break;


			case 6:
				fout<<'_';
				break;
			case 7:
				fout<<'+';
				break;
			}
		}
		fout << std::endl;
	}
}


int cris::map::getWidth()
{
	return mapWidth;
}
int cris::map::getHeight()
{
	return mapHeight;
}
void cris::map::getMenPos(Point*p)
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
void cris::map::nextPos(direction d, Point p, Point *next) {
	
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

bool cris::map::step(direction d)
{
	Point men, n1, next2;

	getMenPos(&men);

	nextPos(d, men,&n1);
	nextPos(d, n1, &next2);
	//std::cout << n1.x;
	//std::cout << n1.y;
	//std::cout << next2.x;
	//std::cout << next2.y;
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
void cris::map::back(direction lastdirection)
{
	Point p;
	Point man;
	getMenPos(&man);
	nextPos(lastdirection, man, &p);
	Point lastpos;
	switch (lastdirection) 
	{
	case up:
		nextPos(down, man, &lastpos);
		break;
	case down:
		nextPos(up, man, &lastpos);
		break;
	case right:
		nextPos(left, man, &lastpos);
		break;
	case left:
		nextPos(right, man, &lastpos);
		break;
	}
	
	/*
		//box		
		case '$' = 1;
		//human	
		case '@' = 2;
		//box on target
		case '*' = 3;
		//wall
		case '#' = 4;
		//target
		case '.' = 5;
		//ground
		case '-':
		case '_':
		case ' ' = 6;
		//man on target	
		case '+' = 7;;
				break;
				*/
	switch (mapContent[man.x][man.y]) 
	{
		//man on target   +
	case 7:
		switch (mapContent[p.x][p.y]) 
		{
			//推过箱子    +$
		case 1:
			switch (mapContent[lastpos.x][lastpos.y])
			{
			case 6://	_+$ => @*_
				mapContent[lastpos.x][lastpos.y] = 2;				
				mapContent[man.x][man.y] = 3;
				mapContent[p.x][p.y] = 6;
				break;
			case 5://	.+$ => +*_
				mapContent[lastpos.x][lastpos.y] = 7;
				mapContent[man.x][man.y] = 3;
				mapContent[p.x][p.y] = 6;
				break;
			}
		case 3://		+*
			switch (mapContent[lastpos.x][lastpos.y])
			{
			case 6://	_+* => @*.
				mapContent[lastpos.x][lastpos.y] = 2;
				mapContent[man.x][man.y] = 3;
				mapContent[p.x][p.y] = 5;
				break;
			case 5://	.+* => +*.
				mapContent[lastpos.x][lastpos.y] = 7;
				mapContent[man.x][man.y] = 3;
				mapContent[p.x][p.y] = 5;
				break;

			}
			//未推过箱子
		case 4:
		case 5:
		case 6:
			switch (mapContent[lastpos.x][lastpos.y]) 
			{
			case 6://	_+| => @.|
				mapContent[lastpos.x][lastpos.y] = 2;
				mapContent[man.x][man.y] = 5;
				break;
			case 5://	.+* => +.|
				mapContent[lastpos.x][lastpos.y] = 7;
				mapContent[man.x][man.y] = 5;
				break;
			}
		
		}
	case 2:
		switch (mapContent[p.x][p.y])
		{
				//推过箱子    @$
			case 1:
				switch (mapContent[lastpos.x][lastpos.y])
				{
				case 6://	_@$ => @$_
					mapContent[lastpos.x][lastpos.y] = 2;
					mapContent[man.x][man.y] = 1;
					mapContent[p.x][p.y] = 6;
					break;
				case 5://	.@$ => +$_
					mapContent[lastpos.x][lastpos.y] = 7;
					mapContent[man.x][man.y] = 1;
					mapContent[p.x][p.y] = 6;
					break;
				}
			case 3://		@*
				switch (mapContent[lastpos.x][lastpos.y])
				{
				case 6://	_@* => @$.
					mapContent[lastpos.x][lastpos.y] = 2;
					mapContent[man.x][man.y] = 1;
					mapContent[p.x][p.y] = 5;
					break;
				case 5://	.@* => +$.
					mapContent[lastpos.x][lastpos.y] = 7;
					mapContent[man.x][man.y] = 1;
					mapContent[p.x][p.y] = 5;
					break;
				}
			//
		case 4:
		case 5:
		case 6:
			switch (mapContent[lastpos.x][lastpos.y])
			{
			case 6://	_+| => @_|
				mapContent[lastpos.x][lastpos.y] = 2;
				mapContent[man.x][man.y] = 6;
				break;
			case 5://	.+* => +_|
				mapContent[lastpos.x][lastpos.y] = 7;
				mapContent[man.x][man.y] = 6;
				break;
			}
		}
	}

}
cris::map::~map()
{
	cleanup();
}
bool cris::map::jump(int i, int j,cris::user &u) 
{

	Point start;
	getMenPos(&start);
	bool visited[50][50] = { false };
	Point end = { i,j };
	Point cur = start;
	std::string lurd;
	bool rt = dfs(start, end,lurd);
	u.lurd += lurd;

	if (rt)
	{

		switch (mapContent[start.x][start.y])
		{
			//人
		case 2:
			switch (mapContent[end.x][end.y])
			{
				//地板
			case 6:
				mapContent[start.x][start.y] = 6;
				mapContent[end.x][end.y] = 2;
				break;
				//目标点
			case 5:
				mapContent[start.x][start.y] = 6;
				mapContent[end.x][end.y] = 7;
				break;
			}
			break;
			//人在目标点
		case 7:
			switch (mapContent[end.x][end.y])
			{
				//地板
			case 6:
				mapContent[start.x][start.y] = 5;
				mapContent[end.x][end.y] = 2;
				break;
				//目标点
			case 5:
				mapContent[start.x][start.y] = 5;
				mapContent[end.x][end.y] = 7;
				break;
			}
			break;
		}
		return true;
	}
	return false;
	
}