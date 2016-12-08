#pragma once

//Enum để lựa chọn công cụ vẽ
enum ShapeMode
{
	LINE,
	DRAWRECT,
	DRAWELLIPSE,
	FILLRECT,
	FILLELLIPSE

};

struct Point
{
	int x;
	int y;
};

Point newPoint(int x, int y);

class CShape 
{
protected:
	int x1;
	int y1;
	int x2;
	int y2;
	COLORREF color;
	ShapeMode type;
	int penStyle;
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d, COLORREF color, int penStyle) = 0;
	virtual ShapeMode getType() = 0;
	void setType(ShapeMode type);
	COLORREF getColor();
	RECT* getDimens();
	void setDimens(RECT* rect);
	void setColor(COLORREF color);
	int getPenStyle();
};