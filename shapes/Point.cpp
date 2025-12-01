/*#include "Point.h"
#include <cmath>

Point::Point(double x, double y)
{
    this->x = x;
    this->y = y;
}

double Point::center_x()
{
    return this->x;
}

double Point::center_y()
{
    return this->y;
}

void Point::update_center_x(int x)
{
    this->x += x;
}

void Point::update_center_y(int y)
{
    this->y += y;
}

ShapeType Point::getType()
{
    return POINT;
}

bool Point::overlap(Shape* tar)
{
    switch (tar->getType())
    {
    case POINT:
        return checkOverlap_pp(this, (Point*)tar);
    case RECTANGLE:
        return checkOverlap_pr(this, (Rectangle*)tar);
    case CIRCLE:
        return checkOverlap_pc(this, (Circle*)tar);
    }
    return false;
}

double Point::Point_dist2(Point* p2)
{
    return (this->x - p2->x) * (this->x - p2->x)
         + (this->y - p2->y) * (this->y - p2->y);
}

double Point::Point_dist(Point* p2)
{
    return std::sqrt(Point_dist2(p2));
}
*/