#include "Point.h"
Point::Point(int x,int y) {
	this->x = x;
	this->y = y;
}
Point::Point(Point & p)
{
	this->x = p.x;
	this->y = p.y;
}
void Point::up(Point*p)
{
	p->x = x-1;
	p->y = y ;
}

void Point::left(Point*p)
{
	p->x = x;
	p->y = y-1;
}

void Point::down(Point*p)
{
	p->x = x + 1;
	p->y = y;
}

void Point::right(Point*p)
{
	p->x = x;
	p->y = y + 1;
}

Point & Point::operator=(Point p)
{
	this->x = p.x;
	this->y = p.y;
	return p;
}
