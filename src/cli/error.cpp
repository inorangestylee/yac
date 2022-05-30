#include <iostream>

void Error(std::string s)
{
	std::cerr << "! > " << s << '\n';
}


void Fatal(std::string s)
{
	Error(s);
	exit(1);
}