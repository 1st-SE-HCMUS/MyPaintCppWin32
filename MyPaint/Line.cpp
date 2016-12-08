#include "stdafx.h"
#include "Line.h"


void CLine::SetData(int a, int b, int c, int d, COLORREF color, int penStyle)
{
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;

	this->type = LINE;
	this->color = color;
	this->penStyle = penStyle;
}

void CLine::Draw(HDC hdc)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

CShape* CLine::Create()
{
	return new CLine;
}

ShapeMode CLine::getType()
{
	return this->type;
}
