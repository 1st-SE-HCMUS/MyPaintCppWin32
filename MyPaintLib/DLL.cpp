#include "stdafx.h"
#include "DLL.h"

namespace PaintLibrary
{

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
		return this->penStyle;
	}


	//////////////////////////////////////////////////////////////////////////


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


	//////////////////////////////////////////////////////////////////////////


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

	//////////////////////////////////////////////////////////////////////////


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
}
