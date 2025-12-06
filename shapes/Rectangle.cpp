/*
#include "Rectangle.h"
#include "Circle.h"
#include "Point.h"
#include "../GAME_ASSERT.h"

Rectangle::Rectangle(double x1, double y1, double x2, double y2): x1(x1), y1(y1), x2(x2), y2(y2) {}

Rectangle* Rectangle::Rectangle_Self() 
{
    return this;
}

double Rectangle::center_x()
{
    return (x1+x2) / 2;
}

double Rectangle::center_y()
{
    return (y1+y2) / 2;
}

void Rectangle::update_center_x(int dx)
{
    x1 += dx;
	x2 += dx;
}

void Rectangle::update_center_y(int dy)
{
    y1 += dy;
	y2 += dy;
}

ShapeType Rectangle::getType()
{
    return RECTANGLE;
}

bool Rectangle::overlap(Shape* tar)
{
    switch (tar->getType())
    {
    case POINT:
    {
        Point* p = static_cast<Point*>(tar);
        return (r * r) >= p->Point_dist2(new Point(x, y));
    }
    case RECTANGLE:
    {
        Rectangle* rct = static_cast<Rectangle*>(tar);
        double cx = std::max(rct->x1, std::min(x, rct->x2));
        double cy = std::max(rct->y1, std::min(y, rct->y2));
        Point cpt(x, y);
        Point rpt(cx, cy);
        return (r * r) >= cpt.Point_dist2(&rpt);
    }
    case CIRCLE:
    {
        Circle* c = static_cast<Circle*>(tar);
        Point p1(x, y);
        Point p2(c->x, c->y);
        double dist2 = p1.Point_dist2(&p2);
        double R = r + c->r;
        return (R * R) >= dist2;
    }
    }

    GAME_ASSERT(false, "Unknown ShapeType.");
    return false;
}

Rectangle *Rectangle_Self(Shape *self)
{
    return (Rectangle *)self->pDerivedObj;
}
Shape *New_Rectangle(double x1, double y1, double x2, double y2){
	return new Rectangle(x1, y1, x2, y2);
}

*/