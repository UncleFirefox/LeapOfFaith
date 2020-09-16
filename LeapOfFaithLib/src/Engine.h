#pragma once
#include <iostream>
class Engine
{
public:
	static int start();
private:
	static void initWindow(std::string wName, const int width, const int height);
};

