#include "stdafx.h"
#include "Rectangle.h"


CShape* CRectangle::Create()
{
	return new CRectangle;
}


void CRectangle::SetData(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, COLORREF color, int penStyle)
{
	x1 = nLeftRect;
	y1 = nTopRect;
	x2 = nRightRect;
	y2 = nBottomRect;
	
	this->type = DRAWRECT;
	this->color = color;
	this->penStyle = penStyle;
}


void CRectangle::Draw(HDC hdc)
{
	if (x1 < x2)
	{
		int tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if (y1 < y2)
	{
		int tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	Rectangle(hdc, x1, y1, x2, y2);
}

ShapeMode CRectangle::getType()
{
	return this->type;
}
