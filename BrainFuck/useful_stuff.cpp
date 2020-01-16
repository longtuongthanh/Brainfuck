#include "useful_stuff.h"

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

Invokable::Invokable(FUNCTION(void, func, void*), void* param)
{
    this->func = func;
    this->param = param;
}

Invokable::~Invokable()
{
    Invoke();
}

void Invokable::Invoke()
{
    func(param);
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
