// MyPaint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyPaint.h"
#include "DLL.h"
#include <windowsx.h>
#include <vector>
#include <string>
#include <fstream>
#include <Commdlg.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_BUFF		255

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

HWND g_hWnd;
HWND hStatusBar;
PaintLibrary::Point p1, p2;
bool isDown = false;
PaintLibrary::ShapeMode currShapeMode = PaintLibrary::DRAWELLIPSE;
vector<PaintLibrary::CShape*> shapes;
PaintLibrary::CShape* currShape = NULL;
COLORREF currColor = RGB(219, 50, 54); 
int currPenSize = 1;
int currPenStyle = PS_SOLID;
HPEN hPen = CreatePen(currPenStyle, currPenSize, currColor);
int windowWidth = 1080, windowHeight = 720;
string defaultFilePath = "data.bin";



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//
void initNewObject();
void saveToFile(string filePath);
void saveToBinaryFile(string filePath);
void loadFromBinaryFile(string filePath);
void openFileDialog();
void saveFileDialog();

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPAINT));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPAINT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
 
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(0, szWindowClass, L"Untitled - My Paint", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
					  INITCOMMONCONTROLSEX icc;
					  icc.dwSize = sizeof(icc);
					  icc.dwICC = ICC_WIN95_CLASSES;
					  InitCommonControlsEx(&icc);

					  g_hWnd = hWnd;

					  HFONT hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
					  hStatusBar = CreateWindowEx(0,
						  STATUSCLASSNAME,
						  L"My Paint",
						  WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
						  0, windowHeight,
						  windowWidth, 20,
						  hWnd, (HMENU)IDC_STATUS_BAR,
						  hInst, NULL);
					  SendMessage(hStatusBar, WM_SETFONT, WPARAM(hFont), TRUE);  
	}
		break;
	case WM_LBUTTONDOWN:
		if (!isDown) 
		{
			p1 = PaintLibrary::newPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			initNewObject();
			isDown = true;
		}
		break;
	case WM_LBUTTONUP:
	{
						 isDown = false;
						 p2 = PaintLibrary::newPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

						 //Vẽ chính thức 
						 switch (currShapeMode)
						 {
						 case PaintLibrary::LINE:
						 {
									  PaintLibrary::CLine* line = new PaintLibrary::CLine;
									  line->SetData(p2.x, p2.y, p1.x, p1.y, currColor, currPenStyle);
									  shapes.push_back(line);
						 }
							 break;
						 case PaintLibrary::DRAWRECT:
						 {
										  PaintLibrary::CRectangle* rect = new PaintLibrary::CRectangle;
										  rect->SetData(p1.x, p1.y, p2.x, p2.y, currColor, currPenStyle);
										  shapes.push_back(rect);
						 }
							 break;
						 case PaintLibrary::DRAWELLIPSE:
						 {
											 PaintLibrary::CEllipse* ellipse = new PaintLibrary::CEllipse;
											 ellipse->SetData(p1.x, p1.y, p2.x, p2.y, currColor, currPenStyle);
											 shapes.push_back(ellipse);
						 }
							 break;
						 case FILLRECT:
							 break;
						 case FILLELLIPSE:
							 break;
						 default:
							 MessageBox(hWnd, L"Lỗi", L"Thông báo", MB_OK | MB_ICONERROR);
						 }

						 InvalidateRect(hWnd, NULL, TRUE);
	}
		break;

	case WM_MOUSEMOVE:
	{
						 int x = GET_X_LPARAM(lParam);
						 int y = GET_Y_LPARAM(lParam);

						 //Status bar
						 WCHAR buffer[128];
						 wsprintf(buffer, L"%d, %dpx", x,y);
						 SetWindowText(hStatusBar, buffer);

						 if (isDown)
						 {
							 p2 = PaintLibrary::newPoint(x, y);
							 InvalidateRect(hWnd, NULL, TRUE); //Invalidate lại window để khi chuyển qua sự kiện paint nó sẽ vẽ các nét vẽ tạm thời
						 }
	}
		break;
		
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEW:
			saveToBinaryFile(defaultFilePath);
			defaultFilePath = "data.bin";
			shapes.clear();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_OPEN:
			openFileDialog();
			break;
		case ID_EDIT_CLEAR:
			shapes.clear();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_EDIT_UNDO:
			shapes.pop_back();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_TOOL_DRAWLINE:
			currShapeMode = PaintLibrary::LINE;
			break;
		case ID_TOOL_DRAWRECTANGLE:
			currShapeMode = PaintLibrary::DRAWRECT;
			break;
		case ID_TOOL_DRAWELLIPSE:
			currShapeMode = PaintLibrary::DRAWELLIPSE;
			break;
		case ID_COLOR_RED:
			currColor = RGB(219, 50, 54);
			break;
		case ID_COLOR_GREEN:
			currColor = RGB(60, 186, 84);
			break;
		case ID_COLOR_BLUE:
			currColor = RGB(72, 133, 237);
			break;
		case ID_COLOR_YELLOW:
			currColor = RGB(244, 194, 13);
			break;
		case ID_FILE_SAVE:
			saveToBinaryFile(defaultFilePath);
			break;
		case ID_FILE_SAVEAS:
			saveFileDialog();
			break;
		case ID_PENSTYLE_PS_SOLID:
			currPenStyle = PS_SOLID;
			break;
		case ID_PENSTYLE_PS_DASH:
			currPenStyle = PS_DASH;
			break;
		case ID_PENSTYLE_PS_DOT:
			currPenStyle = PS_DOT;
			break;
		case ID_PENSTYLE_PS_DASHDOT:
			currPenStyle = PS_DASHDOT;
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...

		for (int i = 0; i < shapes.size(); i++) 
		{
			hPen = CreatePen(shapes[i]->getPenStyle(), currPenSize, shapes[i]->getColor());
			SelectObject(hdc, hPen);
			shapes[i]->Draw(hdc);
		}

		if (isDown)
		{
			//Vẽ nét vẽ tạm thời khi chưa Mouse Up
			hPen = CreatePen(currPenStyle, currPenSize, currColor);
			SelectObject(hdc, hPen);
			currShape->SetData(p2.x, p2.y, p1.x, p1.y, currColor, currPenStyle);
			currShape->Draw(hdc);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
	{
					   PostQuitMessage(0);
	}
		break;

	case WM_CLOSE:
	{
					 int result = MessageBox(hWnd, L"Do you want to save?", L"My Paint", MB_OKCANCEL | MB_ICONQUESTION);
					 if (result == IDOK)
					 {
						 saveFileDialog();
					 }

					 DestroyWindow(hWnd);
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void initNewObject()
{
	switch (currShapeMode)
	{
	case PaintLibrary::LINE:
		currShape = new PaintLibrary::CLine();
		break;
	case PaintLibrary::DRAWRECT:
		currShape = new PaintLibrary::CRectangle();
		break;
	case PaintLibrary::DRAWELLIPSE:
		currShape = new PaintLibrary::CEllipse();
		break;
	case FILLRECT:
		break;
	case FILLELLIPSE:
		break;
	default:
		break;
	}
}


void saveToFile(string filePath)
{
	ofstream myfile;
	myfile.open(filePath, ios::binary);

	if (!myfile.is_open())
	{
		MessageBox(g_hWnd, L"Error open file!", L"My Paint", MB_OK | MB_ICONWARNING);
		return;
	}

	//ok, proceed with output
	int l = shapes.size();
	myfile.write((char*)&l, sizeof(l));

	for (int i = 0; i < shapes.size(); i++)
	{
		myfile.write(reinterpret_cast<const char*>(shapes[i]), sizeof(PaintLibrary::CShape));
	}
	myfile.close();


	//Read
	ifstream inFile;
	inFile.open(filePath, ios::binary);

	if (!inFile.is_open())
	{
		MessageBox(g_hWnd, L"Error open file!", L"My Paint", MB_OK | MB_ICONWARNING);
		return;
	}
	int len = 0;
	inFile.read((char*)&len, sizeof(int));

	while (!inFile.eof() && len > 0)
	{
		char* item_buff = new char[255];
		PaintLibrary::CShape* tmp;
		inFile.read(item_buff, sizeof(PaintLibrary::CShape));
		tmp = reinterpret_cast<PaintLibrary::CShape*>(item_buff);
		//Workaround!
		if (tmp == NULL || &tmp == NULL)
		{
			break;
		}

		switch (tmp->getType())
		{
		case PaintLibrary::LINE:
			shapes.push_back(((PaintLibrary::CLine*)tmp));
			break;
		case PaintLibrary::DRAWRECT:
			shapes.push_back(((PaintLibrary::CRectangle*)tmp));
			break;
		case PaintLibrary::DRAWELLIPSE:
			shapes.push_back(((PaintLibrary::CEllipse*)tmp));
			break;
		case FILLRECT:
			break;
		case FILLELLIPSE:
			break;
		default:
			break;
		}
		
		len--;
	}
	inFile.close();
}


void saveToBinaryFile(string filePath) {
	std::ofstream out;
	out.open(filePath, std::iostream::out | std::iostream::binary | std::iostream::trunc);

	if (out.is_open()) {
		int size = shapes.size();
		out.write(reinterpret_cast<const char *>(&size), sizeof(size));

		for (PaintLibrary::CShape* shape : shapes) {
			int type = shape->getType();
			COLORREF color = shape->getColor();
			RECT* rect = shape->getDimens();
			out.write(reinterpret_cast<const char*>(&type), sizeof(type));
			out.write(reinterpret_cast<const char*>(&color), sizeof(COLORREF));
			out.write(reinterpret_cast<const char*>(rect), sizeof(RECT));
		}
	}
	else {
		OutputDebugString(L"Can't open data.bin to write");
	}

	out.close();
}

void loadFromBinaryFile(string filePath) {
	std::ifstream in;
	in.open(filePath, std::iostream::in | std::iostream::binary);

	if (in.is_open()) {
		char* buffer = new char[MAX_BUFF];
		int size;
		in.read(buffer, sizeof(size));

		size = buffer[0];
		shapes.clear();


		for (int i = 0; i < size; i++) 
		{
			char* item_buff = new char[MAX_BUFF];

			PaintLibrary::CShape* shape = NULL;

			int type;
			COLORREF color;
			in.read(item_buff, sizeof(type));
			type = item_buff[0];
			in.read(item_buff, sizeof(COLORREF));
			color = item_buff[0];
			int r = GetRValue(color);
			color = item_buff[1];
			int g = GetGValue(color);
			color = item_buff[2];
			int b = GetBValue(color);

			color = RGB(r, g, b);

			switch (type)
			{
			case PaintLibrary::LINE:
				shape = new PaintLibrary::CLine();
				break;
			case PaintLibrary::DRAWRECT:
				shape = new PaintLibrary::CRectangle();
				break;
			case PaintLibrary::DRAWELLIPSE:
				shape = new PaintLibrary::CEllipse();
				break;
			case FILLRECT:
				break;
			case FILLELLIPSE:
				break;
			default:
				break;
			}

			shape->setType((PaintLibrary::ShapeMode)type);
			shape->setColor(color);
		
			RECT* rect;
			in.read(item_buff, sizeof(RECT));
			rect = reinterpret_cast<RECT*>(item_buff);
			shape->setDimens(rect);

			shapes.push_back(shape);

			delete[] item_buff;
			item_buff = NULL;
		}

		delete[] buffer;
	}
	else {

		OutputDebugString(L"Can't open data.bin to read");
	}

	in.close();
}

void openFileDialog()
{
	OPENFILENAME ofn;
	WCHAR szFilePath[MAX_PATH] = L"";
	WCHAR szFileTitle[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"bin";

	if (GetOpenFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName 
		wstring ws(szFilePath);
		// your new String
		string fileName(ws.begin(), ws.end());
		loadFromBinaryFile(fileName);
		InvalidateRect(g_hWnd, NULL, TRUE);
	}
}

void saveFileDialog()
{
	OPENFILENAME ofn;
	WCHAR szFileName[MAX_PATH] = L"";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"bin";

	if (GetSaveFileName(&ofn))
	{
		// Do something usefull with the filename stored in szFileName 
		wstring ws(szFileName);
		// your new String
		string fileName(ws.begin(), ws.end());

		defaultFilePath = fileName;
		saveToBinaryFile(fileName);
	}
}