#pragma once
#include "Shape.h"

class CLine : public CShape {
public:
public:
	void Draw(HDC hdc);
	CShape* Create();
	void SetData(int a, int b, int c, int d, COLORREF color, int penStyle);
	ShapeMode getType();
};