#include "ArchDemos.h"

static bool bRunning = true;

int main(int argc, const char* argv[])
{
	(void)argc;
	(void)argv;

	OutputDebugString("TEST -- ARCHDEMOS -- INIT.\n");

	int ExitCode = 0;

	if (!glfwInit())
	{
        OutputDebugString("ERROR: glfwInit FAILED!\n");
		ExitCode = 1;
	}
	else
	{

	}

	return ExitCode;
}
