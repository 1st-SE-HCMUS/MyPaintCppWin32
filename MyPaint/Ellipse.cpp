#include "stdafx.h"
#include "Ellipse.h"


CShape* CEllipse::Create()
{
	return new CEllipse;
}

void CEllipse::SetData(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, COLORREF color, int penStyle)
{
	x1 = nLeftRect;
	y1 = nTopRect;
	x2 = nRightRect;
	y2 = nBottomRect;

	this->type = DRAWELLIPSE;
	this->color = color;
	this->penStyle = penStyle;
}

void CEllipse::Draw(HDC hdc)
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

	Ellipse(hdc, x1, y1, x2, y2);
}

ShapeMode CEllipse::getType()
{
	return this->type;
}
