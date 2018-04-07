#pragma once
class Point
{

public:
	int x;
	int y;
	Point() = default;
	Point(int x, int y);
	Point(Point &p);
	void up(Point*);
	void left(Point*);

	void down(Point*);
	void right(Point*);
	Point &operator=(Point p);
};
