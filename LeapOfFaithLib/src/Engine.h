#pragma once
class Engine
{
public:
	static int start();
private:
	static void initWindow(const char* wName, const int width, const int height);
	static void initGlobals();
};

