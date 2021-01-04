#include "minCircle.h"
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

  
// calculates the distance between 2 points.
float distance(const Point& p1, const Point& p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// checks if a point is within a circle
bool isPointInCircle(Point *p, const Circle& c) {
	if (distance(*p, c.center) <= c.radius) {
		return true;
	}
	return false;
}

Circle circleFromTwoPoints(Point p1, Point p2) {
	float x = (p1.x + p2.x) / 2;
	float y = (p1.y + p2.y) / 2;
	Point center(x, y);

	float radius = distance(p1, p2) / 2;
	return Circle(center, radius);
}

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
Circle circleFromThreePoints(const Point& p1, const Point& p2, const Point& p3) {
	float x1 = p1.x;
	float x2 = p2.x;
	float x3 = p3.x;
	float y1 = p1.y;
	float y2 = p2.y;
	float y3 = p3.y;

	float m = 0.5 * ((pow(x1, 2) - pow(x2, 2)) + (pow(y1, 2) - pow(y2, 2)));
	float n = 0.5 * ((pow(x2, 2) - pow(x3, 2)) + (pow(y2, 2) - pow(y3, 2)));

	float b = ((x1-x2) * n - (x2-x3) * m) / ((x2-x3)*(y2-y1) - (x1-x2)*(y3-y2));
	float a = (m + b*(y2-y1)) / (x1-x2);

	Point center(a, b);
	float radius = distance(center, p1);

	return Circle(center, radius);
}

// checks if a given circle contains all points
bool isValidCircle(const Circle& c, vector<Point*> points) {
	for (int i = 0; i < points.size(); i++) {
		if (!isPointInCircle(points[i], c))
			return false;
	}
	return true;
}

// returns the trivial circle when size <= 3
Circle trivialCircle(vector<Point*> points) {
	if (points.empty())
		return Circle(Point(0, 0), 0);
	if (points.size() == 1)
		return Circle(*(points[0]), 0);
	if (points.size() == 2)
		return circleFromTwoPoints(*(points[0]), *(points[1]));

	// check every pair of points to find a circle containing all three.
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = i + 1; i < points.size(); i++) {
			Circle c = circleFromTwoPoints(*(points[i]), *(points[j]));
			if (isValidCircle(c, points))
				return c;
		}
	}
	
	return circleFromThreePoints(*(points[0]), *(points[1]), *(points[2]));
}

void swap(Point*& a, Point*& b) {
	Point* temp = b;
	b = a;
	a = temp;
}

Circle welzl_helper(Point** points, size_t size, vector<Point*> R) {
	// If |R| = 3 we are done. if n = 0 then |R| = 0 and there is to nothing be done.
	if (size == 0 || R.size() == 3)
		return trivialCircle(R);

 	// Pick a random point 
    int randomIndex = rand() % size; 
    Point *p = points[randomIndex];
  
    // Put the picked point at the end of the points array
    swap(points[randomIndex], points[size - 1]); 
  
    // Get the minimal circle from the set of points without p
    Circle c = welzl_helper(points, size - 1, R); 
  
    // If c contains p, it is the minimal circle.
    if (isPointInCircle(p, c))
        return c; 
  
    // Otherwise, p must be on the circumference of the minimal circle
    R.push_back(p); 
  
    // Return the minimal cricle for R U {p} 
    return welzl_helper(points, size - 1, R);
}

// implement
Circle findMinCircle(Point** points, size_t size){
	vector<Point*> pointsOnCircumference;

	return welzl_helper(points, size, pointsOnCircumference);
}
