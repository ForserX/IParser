// IParser.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "IParserSystem.hpp"
#include <Windows.h>
#include <iostream>

int main()
{
	IFS::config ini = IFS::config("../test.ltx");
	std::cout << ini.get_value("sect", "key") << std::endl;
	system("pause");
    return 0;
}

