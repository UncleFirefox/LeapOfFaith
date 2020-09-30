#pragma once
#include <iostream>
class Engine
{
public:
	static int start();
private:
	static void initWindow(const std::string& wName, const int width, const int height);
	static void initGlobals();
};

