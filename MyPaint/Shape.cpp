#include "stdafx.h"
#include "Shape.h"


Point newPoint(int x, int y)
{
	Point p;
	p.x = x;
	p.y = y;

	return p;
}

COLORREF CShape::getColor()
{
	return this->color;
}

void CShape::setType(ShapeMode type)
{
	this->type = type;
}

RECT* CShape::getDimens()
{
	RECT* rect = new RECT();
	rect->left = x1;
	rect->top = y1;
	rect->right = x2;
	rect->bottom = y2;

	return rect;
}

void CShape::setDimens(RECT* rect)
{
	this->x1 = rect->left;
	this->x2 = rect->right;
	this->y1 = rect->top;
	this->y2 = rect->bottom;
}

void CShape::setColor(COLORREF color)
{
	this->color = color;
}

int CShape::getPenStyle()
{
	return penStyle;
}
