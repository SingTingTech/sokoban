#include "Point.h"
cris::Point::Point(int x,int y) {
	this->x = x;
	this->y = y;
}
cris::Point::Point(const Point & p)
{
	this->x = p.x;
	this->y = p.y;
}
bool cris::Point::operator==(const Point & p)
{
	if (this->x == p.x&&this->y == p.y)
		return true;
	return false;
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
void cris::Point::up(Point&p)const
{
	p.x = x - 1;
	p.y = y;
}

void cris::Point::left(Point&p)const
{
	p.x = x;
	p.y = y - 1;
}

void cris::Point::down(Point&p)const
{
	p.x = x + 1;
	p.y = y;
}

void cris::Point::right(Point&p)const
{
	p.x = x;
	p.y = y + 1;
}

void cris::Point::move(direction d)
{
	switch (d) 
	{
	case cris::up:
		up();
	case cris::down:
		down();
	case cris::right:
		right();
	case cris::left:
		left();
	}
}

void cris::Point::move(Point & p, direction d) const
{
	switch (d)
	{
	case cris::up:
		up(p);
		break;
	case cris::down:
		down(p);
		break;
	case cris::right:
		right(p);
		break;
	case cris::left:
		left(p);
		break;
	}
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
