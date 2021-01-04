// 31578622	31601262
// We used Welzl's algorithm, taken from Wikipedia.
/* 
	algorithm welzl:
		input: Finite sets P and R of points in the plane |R|≤ 3.
		output: Minimal disk enclosing P with R on the boundary.

		if P is empty or |R| = 3 then
			return trivial(R)
		choose p in P (randomly and uniformly)
		D := welzl(P - { p }, R)
		if p is in D then
			return D

		return welzl(P - { p }, R ∪ { p })
*/
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <vector>
#include "anomaly_detection_util.h"

using namespace std;

// ------------ DO NOT CHANGE -----------
//class Point{
//public:
//	float x,y;
//	Point(float x,float y):x(x),y(y){}
//};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------
  
// calculates the distance between 2 points.
float distance(const Point& p1, const Point& p2);

// checks if a point is within a circle
bool isPointInCircle(Point *p, const Circle& c);

Circle circleFromTwoPoints(Point p1, Point p2);

/*
	p1 = (x1, y1)
	p2 = (x2, y2)
	p3 = (x3, y3)
	center = (a, b)

	all points should have equal distance from the center point (aka the radius):
	p1c = p2c = p3c = r

	so we can get 2 equiations:
	eq1: a(x2-x1) + b(y2-y1) + 1/2[(x1^2 - x2^2) + (y1^2-y2^2)] = 0
	eq2: a(x3-x2) + b(y3-y2) + 1/2[(x2^2 - x3^2) + (y2^2-y3^2)] = 0
	
	solve for a,b:
	m = 1/2[(x1^2 - x2^2) + (y1^2-y2^2)]
	n = 1/2[(x2^2 - x3^2) + (y2^2-y3^2)]

	from eq1 we get:
	eq3: a = [-m - b(y2-y1)] / (x2-x1) = [m + b(y2-y1)] / (x1-x2)
	from eq2 we get:
	eq4: a = [-n - b(y3-y2)] / (x3-x2) = [n + b(y3-y2)] / (x2-x3)

	eq3 = eq4, so:
	[m + b(y2-y1)] / (x1-x2) = [n + b(y3-y2)] / (x2-x3)
	
	there for:
	b = [x2-x1]n - (x2-x3)m] / [(x2-x3)(y2-y1) - (x1-x2)(y3-y2)]

	substitute b to eq3 and get a.
*/
Circle circleFromThreePoints(const Point& p1, const Point& p2, const Point& p3);

// checks if a given circle contains all points
bool isValidCircle(const Circle& c, vector<Point*> points);

// returns the trivial circle when size <= 3
Circle trivialCircle(vector<Point*> points);

void swap(Point*& a, Point*& b);

Circle welzl_helper(Point** points, size_t size, vector<Point*> R);

// implement
Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */