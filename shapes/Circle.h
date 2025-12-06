#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "Shape.h"

/**
 * @see Shape.cpp
 */
class Circle : public Shape
{
public:
	double x, y, r;	
	bool overlap(const Shape &s) const;
	double center_x() const { return x; }
	double center_y() const { return y; }
	const ShapeType getType() const { return ShapeType::CIRCLE; }
	
	Circle(double x, double y, double r) : x{x}, y{y}, r{r} {}
	Circle(int x, int y, int r) :
		x{static_cast<double>(x)},
		y{static_cast<double>(y)},
		r{static_cast<double>(r)} {}

	void update_position(double dx, double dy) override {
		x += dx;
		y += dy;
	}
};

#endif
