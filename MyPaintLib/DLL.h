#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define PAINTLIBRARY_API __declspec(dllexport) 
#else
#define PAINTLIBRARY_API __declspec(dllimport) 
#endif

namespace PaintLibrary
{
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

	PAINTLIBRARY_API Point newPoint(int x, int y);

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
		PAINTLIBRARY_API virtual void Draw(HDC hdc) = 0;
		PAINTLIBRARY_API virtual CShape* Create() = 0;
		PAINTLIBRARY_API virtual void SetData(int a, int b, int c, int d, COLORREF color, int penStyle) = 0;
		PAINTLIBRARY_API virtual ShapeMode getType() = 0;
		PAINTLIBRARY_API void setType(ShapeMode type);
		PAINTLIBRARY_API COLORREF getColor();
		PAINTLIBRARY_API RECT* getDimens();
		PAINTLIBRARY_API void setDimens(RECT* rect);
		PAINTLIBRARY_API void setColor(COLORREF color);
		PAINTLIBRARY_API int getPenStyle();
	};

	class CLine : public CShape {
	public:
	public:
		PAINTLIBRARY_API  void Draw(HDC hdc);
		PAINTLIBRARY_API CShape* Create();
		PAINTLIBRARY_API void SetData(int a, int b, int c, int d, COLORREF color, int penStyle);
		PAINTLIBRARY_API ShapeMode getType();
	};

	class CEllipse : public CShape {
	private:
	public:
		PAINTLIBRARY_API void Draw(HDC hdc);
		PAINTLIBRARY_API CShape* Create();
		PAINTLIBRARY_API void SetData(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, COLORREF color, int penStyle);
		PAINTLIBRARY_API ShapeMode getType();
	};

	class CRectangle : public CShape {
	private:
	public:
		PAINTLIBRARY_API void Draw(HDC hdc);
		PAINTLIBRARY_API CShape* Create();
		PAINTLIBRARY_API void SetData(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect, COLORREF color, int penStyle);
		PAINTLIBRARY_API ShapeMode getType();
	};
}
