
/*#include "Circle.h"
#include "Point.h"
#include "Rectangle.h"
#include "../GAME_ASSERT.h"

Circle::Circle(double x, double y, double r): x(x), y(y), r(r){}

Circle* Circle::Circle_Self() 
{
    return this;
}

double Circle::center_x()
{
    return x;
}

double Circle::center_y()
{
    return y;
}

void Circle::update_center_x(int dx)
{
    x += dx;
}

void Circle::update_center_y(int dy)
{
    y += dy;
}

ShapeType Circle::getType()
{
    return CIRCLE;
}

bool Circle::overlap(Shape* tar)
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

Circle *Circle_Self(Shape *self)
{
    return (Circle *)self->pDerivedObj;
}
// factory
Shape* New_Circle(double x, double y, double r)
{
    return new Circle(x, y, r);
}
*/
