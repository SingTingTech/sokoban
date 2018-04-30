#pragma once
#include"direction.h"
namespace cris{
	class Point
	{

	public:
		int x;
		int y;
		Point() = default;
		Point(int x, int y);
		void up(Point*);
		void left(Point*);
		void down(Point*);
		void right(Point*);
		void up(Point&)const;
		void left(Point&)const;
		void down(Point&)const;
		void right(Point&)const;
		void move(direction d);
		void move(Point &p,direction d)const;
		void up();
		void left();
		void down();
		void right();
		Point(const Point&p);
		bool operator==(const Point &p);
		Point &operator=(const Point &p);
	};
}