// IParser.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "../IParserLib/IParserSystem.hpp"
#include <Windows.h>
#include <iostream>

int main()
{
	config ini = config("../test.ltx");
	std::cout << ini.get_logic("sect", "key_parent") << std::endl;
	system("pause");
    return 0;
}

