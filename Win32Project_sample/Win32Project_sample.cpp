// Win32Project_sample.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project_sample.h"

#include <windows.h>
#include <GL/gl.h>
#include "Header.h"
#include "Controller.h"

LPCWSTR className = L"OpenGL";
LPCWSTR windowName = L"OpenGL Cube";
int winX = 0, winY = 0;
int winWidth = 512, winHeight = 512;
Controller states;

HWND hWnd;
HDC hDC;
HGLRC hGLRC;
HPALETTE hPalette;

void
init(void) {
	///* set viewing projection */
	//glMatrixMode(GL_PROJECTION);
	//glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);

	///* position viewer */
	//glMatrixMode(GL_MODELVIEW);
	//glTranslatef(0.0F, 0.0F, -2.0F);

	///* position object */
	//glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
	//glRotatef(30.0F, 0.0F, 1.0F, 0.0F);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	const auto light0_position = glm::vec4(-1.f, -1.f, -1.f, 0.f);
	const auto light0_ambient = glm::vec4(0.f, 0.f, 0.f, 1.f);
	const auto light0_diffuse = glm::vec4(0.01f, 0.01f, 0.01f, 1.f);
	const auto light0_specular = glm::vec4(0.01f, 0.01f, 0.01f, 1.f);
	glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(light0_position));
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(light0_ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(light0_diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(light0_specular));

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	const auto mat_specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.f);
	const auto mat_emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(mat_specular));
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(mat_emission));

	glClearColor(1.f, 1.f, 1.f, 1.f);
}

void
redraw(void) {
	/* clear color and depth buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static auto angle = 0.0f;
	states.m_rotation = glm::angleAxis<float>( glm::pi<float>() * angle, glm::normalize(glm::vec3(1, 1, 0)) );
	angle += 0.01f;
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), states.m_pan)
		* glm::toMat4(states.m_rotation)
		* glm::scale(glm::mat4(1.0f), states.m_scale);

	states.m_viewModelMat = states.m_camera.GetViewMatrix() * modelMatrix;
	states.m_projectionMat = states.m_camera.GetProjectionMatrix();
	states.m_projectionMatInv = glm::inverse(states.m_projectionMat);
	states.m_mvp = states.m_projectionMat * states.m_viewModelMat;
	states.m_mvpInv = glm::inverse(states.m_mvp);

	glLoadMatrixf(glm::value_ptr(states.m_mvp));
	//static float angle = 1.f;
	//glRotatef(angle, 1.0F, 1.0F, 1.0F);
	
	/* draw six faces of a cube */
	glColor3f(0, 0.5f, 0);
	glBegin(GL_QUADS);
	glNormal3f(0.0F, 0.0F, 1.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);
	glVertex3f(-0.5F, -0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);

	glNormal3f(0.0F, 0.0F, -1.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glVertex3f(0.5F, 0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);

	glNormal3f(0.0F, 1.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, 0.5F, -0.5F);
	glVertex3f(-0.5F, 0.5F, -0.5F); glVertex3f(-0.5F, 0.5F, 0.5F);

	glNormal3f(0.0F, -1.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(0.5F, -0.5F, -0.5F);
	glVertex3f(0.5F, -0.5F, 0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);

	glNormal3f(1.0F, 0.0F, 0.0F);
	glVertex3f(0.5F, 0.5F, 0.5F); glVertex3f(0.5F, -0.5F, 0.5F);
	glVertex3f(0.5F, -0.5F, -0.5F); glVertex3f(0.5F, 0.5F, -0.5F);

	glNormal3f(-1.0F, 0.0F, 0.0F);
	glVertex3f(-0.5F, -0.5F, -0.5F); glVertex3f(-0.5F, -0.5F, 0.5F);
	glVertex3f(-0.5F, 0.5F, 0.5F); glVertex3f(-0.5F, 0.5F, -0.5F);
	glEnd();

	SwapBuffers(hDC);
	InvalidateRect(hWnd, NULL, FALSE);
}

void
resize(void) {
	/* set viewport to cover the window */
	glViewport(0, 0, winWidth, winHeight);
}

void
setupPixelFormat(HDC hDC) {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		16,                             /* prefered color depth */
		0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
		0,                              /* no alpha buffer */
		0,                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		16,                             /* depth buffer */
		0,                              /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};
	int pixelFormat;

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat == 0) {
		MessageBox(WindowFromDC(hDC), L"ChoosePixelFormat failed.", L"Error",
			MB_ICONERROR | MB_OK);
		exit(1);
	}

	if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) {
		MessageBox(WindowFromDC(hDC), L"SetPixelFormat failed.", L"Error",
			MB_ICONERROR | MB_OK);
		exit(1);
	}
}

//void
//setupPalette(HDC hDC) {
//	int pixelFormat = GetPixelFormat(hDC);
//	PIXELFORMATDESCRIPTOR pfd;
//	LOGPALETTE* pPal;
//	int paletteSize;
//
//	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
//
//	if (pfd.dwFlags & PFD_NEED_PALETTE) {
//		paletteSize = 1 << pfd.cColorBits;
//	}
//	else {
//		return;
//	}
//
//	pPal = (LOGPALETTE*)
//		malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
//	pPal->palVersion = 0x300;
//	pPal->palNumEntries = paletteSize;
//
//	/* build a simple RGB color palette */
//	{
//		int redMask = (1 << pfd.cRedBits) - 1;
//		int greenMask = (1 << pfd.cGreenBits) - 1;
//		int blueMask = (1 << pfd.cBlueBits) - 1;
//		int i;
//
//		for (i = 0; i<paletteSize; ++i) {
//			pPal->palPalEntry[i].peRed =
//				(((i >> pfd.cRedShift) & redMask) * 255) / redMask;
//			pPal->palPalEntry[i].peGreen =
//				(((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
//			pPal->palPalEntry[i].peBlue =
//				(((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
//			pPal->palPalEntry[i].peFlags = 0;
//		}
//	}
//
//	hPalette = CreatePalette(pPal);
//	free(pPal);
//
//	if (hPalette) {
//		SelectPalette(hDC, hPalette, FALSE);
//		RealizePalette(hDC);
//	}
//}

LRESULT APIENTRY
WndProc(
HWND hWnd,
UINT message,
WPARAM wParam,
LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		/* initialize OpenGL rendering */
		hDC = GetDC(hWnd);
		setupPixelFormat(hDC);
		//setupPalette(hDC);
		hGLRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hGLRC);
		init();
		return 0;
	case WM_DESTROY:
		/* finish OpenGL rendering */
		if (hGLRC) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hGLRC);
		}
		if (hPalette) {
			DeleteObject(hPalette);
		}
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		/* track window size changes */
		if (hGLRC) {
			winWidth = (int)LOWORD(lParam);
			winHeight = (int)HIWORD(lParam);
			resize();
			return 0;
		}
	//case WM_PALETTECHANGED:
	//	/* realize palette if this is *not* the current window */
	//	if (hGLRC && hPalette && (HWND)wParam != hWnd) {
	//		UnrealizeObject(hPalette);
	//		SelectPalette(hDC, hPalette, FALSE);
	//		RealizePalette(hDC);
	//		redraw();
	//		break;
	//	}
	//	break;
	//case WM_QUERYNEWPALETTE:
	//	/* realize palette if this is the current window */
	//	if (hGLRC && hPalette) {
	//		UnrealizeObject(hPalette);
	//		SelectPalette(hDC, hPalette, FALSE);
	//		RealizePalette(hDC);
	//		redraw();
	//		return TRUE;
	//	}
	//	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		if (hGLRC) {
			redraw();
		}
		EndPaint(hWnd, &ps);
		return 0;
	}
	break;
	case WM_CHAR:
		/* handle keyboard input */
		switch ((int)wParam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			return 0;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY
WinMain(
HINSTANCE hCurrentInst,
HINSTANCE hPreviousInst,
LPSTR lpszCmdLine,
int nCmdShow) {
	WNDCLASS wndClass;
	MSG msg;

	/* register window class */
	wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hCurrentInst;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className;
	RegisterClass(&wndClass);

	/* create window */
	hWnd = CreateWindow(
		className, windowName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		winX, winY, winWidth, winHeight,
		NULL, NULL, hCurrentInst, NULL);

	/* display window */
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* process messages */
	while (GetMessage(&msg, NULL, 0, 0) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}