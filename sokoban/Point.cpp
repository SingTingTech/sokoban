#include "Point.h"
cris::Point::Point(int x,int y) {
	this->x = x;
	this->y = y;
}
cris::Point::Point(Point & p)
{
	this->x = p.x;
	this->y = p.y;
}
void cris::Point::up(Point*p)
{
	p->x = x-1;
	p->y = y ;
}

void cris::Point::left(Point*p)
{
	p->x = x;
	p->y = y-1;
}

void cris::Point::down(Point*p)
{
	p->x = x + 1;
	p->y = y;
}

void cris::Point::right(Point*p)
{
	p->x = x;
	p->y = y + 1;
}
void cris::Point::up(Point&p)
{
	p.x = x - 1;
	p.y = y;
}

void cris::Point::left(Point&p)
{
	p.x = x;
	p.y = y - 1;
}

void cris::Point::down(Point&p)
{
	p.x = x + 1;
	p.y = y;
}

void cris::Point::right(Point&p)
{
	p.x = x;
	p.y = y + 1;
}

void cris::Point::up()
{
	x = x - 1;
	y = y;
}

void cris::Point::left()
{
	x = x;
	y = y - 1;
}

void cris::Point::down()
{
	x = x + 1;
	y = y;
}

void cris::Point::right()
{
	x = x;
	y = y + 1;
}



cris::Point & cris::Point::operator=(const Point &p)
{
	this->x = p.x;
	this->y = p.y;
	return (*this);
}
