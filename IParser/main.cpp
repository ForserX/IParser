// IParser.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "../IParserLib/IParserSystem.hpp"
#include <Windows.h>
#include <iostream>

int main()
{
	config ini = config("../test.ltx");
	std::cout << ini.get_number("sect", "key") << std::endl;
	std::cout << ini.get_logic("sect", "logic") << std::endl;
	std::cout << ini.get_float("sect", "key_parent") << std::endl;
	system("pause");
    return 0;
}

