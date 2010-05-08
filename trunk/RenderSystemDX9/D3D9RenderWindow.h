#pragma once
#include "RenderWindow.h"

class DLL_EXPORT CD3D9RenderWindow: public CRenderWindow
{
public:
	CD3D9RenderWindow();
	~CD3D9RenderWindow();
public:
	bool Create(WNDPROC pWndProc, const std::wstring& wstrWindowTitle, int32 nWidth, int32 nHeight, bool bFullScreen = false); 
	void SetFullscreen(bool bFullScreen, uint32 width, uint32 height);
	double GetTime();
	float GetElapsedTime();
	bool FrameBegin();
	void FrameEnd();
	HWND GetHWND();
	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	bool Init(bool bParseCommandLine, bool bShowMsgBoxOnError, bool bHandleAltEnter);
	bool m_bActive;
	bool m_bMoving;
	bool m_bMinimized;
};