#include <iostream>
#include "calculator.h"
#include "error.h"

using namespace std;

const string PROMPT = "? > ";
const string RESULT = "= > ";

void show_help()
{
	cout << "commands:" << endl
		<< "  :about - about program and author" << endl
		<< "  :help :h - help (this screen)" << endl
		<< "  :quit :q - quit" << endl
		<< endl;
}

void show_about()
{
	cout << "YetAnotherCalculator" << endl
		<< "by Oleg Galushko (2022)" << endl 
		<< endl;
}

void run_command(string c) {
	if (c == "quit" || c == "q") {
		exit(0);
	}
	if (c == "help" || c == "h") {
		show_help();
		return;
	}
	if (c == "about") {
		show_about();
		return;
	}
	string error_msg = "unkonwn command: " + c;
	Error(error_msg);
}

void commandline()
{
	char ch;
	cout << PROMPT;
	while (cin >> ch) {
		switch (ch) {
		case ':':
		{
			string command;
			while (cin.get(ch) && isalpha(ch)) {
				command += ch;
			}
			run_command(command);
			return;
		}
		default:
		{
			cin.putback(ch);
			calculator::calculate();
		}
		}
	}
}



int main()
{
	//calculator::VariableTable vt = calculator::VariableTable();

	//cout << RESULT << vt.get("PI") << endl;
	//cout << RESULT << vt.get("E") << endl;

	while (true) { commandline(); }

	return 0;
}




