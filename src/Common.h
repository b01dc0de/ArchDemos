#ifndef ARCHFILES_COMMON_H
#define ARCHFILES_COMMON_H

/* ----- BEGIN PLATFORM INCLUDES ----- */
// WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/* -----  END  PLATFORM INCLUDES ----- */

/* ----- BEGIN C/C++ STD LIB INCLUDES ----- */
#include <cstdio>
#include <string>
/* -----  END  C/C++ STD LIB INCLUDES ----- */

/* ----- BEGIN OPENGL INCLUDES ----- */
//#include <gl/GL.h>
//#include <gl/GLU.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
/* -----  END  OPENGL INCLUDES ----- */
/* ----- BEGIN LIB INCLUDES ----- */
#include <HandmadeMath.h>
#include "stb_image.h"
/* -----  END  LIB INCLUDES ----- */
/* ----- BEGIN ARCHDEMOS INCLUDES ----- */
#include "Math.h"
#include "Utils.h"
/* -----  END  ARCHDEMOS INCLUDES ----- */


#endif // ARCHFILES_COMMON_H