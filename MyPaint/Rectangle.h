#pragma once
#include "Shape.h"

class CRectangle : public CShape {
private:
public:
	void Draw(HDC hdc);
	CShape* Create();
	void SetData(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, COLORREF color, int penStyle);
	ShapeMode getType();
};