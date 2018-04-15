#pragma once
namespace cris{
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
		void up(Point&);
		void left(Point&);
		void down(Point&);
		void right(Point&);
		void up();
		void left();
		void down();
		void right();
		Point(const Point&p) 
		{
			this->x = p.x;
			this->y = p.y;
		}
		Point &operator=(const Point &p);
	};
}