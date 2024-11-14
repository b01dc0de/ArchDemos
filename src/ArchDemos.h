#ifndef ARCHDEMOS_H
#define ARCHDEMOS_H

/* ----- BEGIN WIN PLATFORM INCLUDES ----- */
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
/* -----  END  WIN PLATFORM INCLUDES ----- */

/* ----- BEGIN C/C++ STD LIB INCLUDES ----- */
#include <cstdio>
/* -----  END  C/C++ STD LIB INCLUDES ----- */

int WINAPI WinMain
(
	HINSTANCE hInst,
	HINSTANCE hPrevInst,
	PSTR CmdLine,
	int ShowWnd
);

#endif // ARCHDEMOS_H