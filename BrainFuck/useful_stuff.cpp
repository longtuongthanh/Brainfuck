#include "useful_stuff.h"
#include "Input.h"

NonCopyable::NonCopyable()
{
    //ctor
}

NonCopyable::~NonCopyable()
{
    //dtor
}

NonCopyable::NonCopyable(NonCopyable&)
{

}

NonCopyable& NonCopyable::operator=(const NonCopyable&)
{
	return *this;
}

double Point::length()
{
    return sqrt(x*x + y*y);
}

double Point::operator*(const Point& p) const
{
    return x * p.x + y * p.y;
}

Point Point::operator*(double k) const
{
    return Point(x*k, y*k);
}

Point Point::operator-(const Point& p) const
{
    return Point(x-p.x, y-p.y);
}

Point Point::operator+(const Point& p) const
{
    return Point(x+p.x, y+p.y);
}

Point& Point::operator+=(const Point& p)
{
    this->x += p.x;
    this->y += p.y;

    return *this;
}

bool Point::operator==(const Point& right)
{
	return this->x == right.x && this->y == right.y;
}

bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;
    return false;
}

int orientation(Point p, Point q, Point r)
{
    float val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear 
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case 
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and p2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases 
}

bool isInside(std::vector<Point>& polygon, Point p)
{
    // There must be at least 3 vertices in polygon[] 
    if (polygon.size() < 3)  return false;
    // Create a point for line segment from p to infinite 
    Point extreme = { 100, p.y };

    // Count intersections of the above line with sides of polygon 
    int count = 0, i = 0;
    do
    {
        int next = (i + 1) % polygon.size();

        // Check if the line segment from 'p' to 'extreme' intersects 
        // with the line segment from 'polygon[i]' to 'polygon[next]' 
        if (doIntersect(polygon[i], polygon[next], p, extreme))
        {
            // If the point 'p' is colinear with line segment 'i-next', 
            // then check if it lies on segment. If it lies, return true, 
            // otherwise false 
            if (orientation(polygon[i], p, polygon[next]) == 0)
                return onSegment(polygon[i], p, polygon[next]);

            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise 
    return count & 1;  // Same as (count%2 == 1) 
}

void DragAndDrop::UpdateDragAndDrop(Point& pos, std::vector<Point> hitBox, Input& input, Point camPos)
{
    if (beingHold)
    {
        if (input.MouseFlag() & 0x0001)
        {
            pos += input.MouseToScreen() + camPos - prevPos;
        }
        else
        {
            beingHold = false;
        }
    }
    else if(input.MouseFlag() & 0x0001 && isInside(hitBox, input.MouseToScreen() + camPos))
    {
        isInside(hitBox, input.MouseToScreen() + camPos);
        beingHold = true;
    }
    prevPos = input.MouseToScreen() + camPos;
}