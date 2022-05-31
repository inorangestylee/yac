#include <iostream>

#include "calculator.h"
#include "error.h"


using namespace std;

const string MOTD = "Yet another calculator v0.0.1\n"
"-----------------------------\n"
"type :help for help\n\n";
						
const string PROMPT = "? > ";
const string RESULT = "= > ";


void show_help()
{
	cout << "commands:" << endl
		<< "  :about - about program and author" << endl
		<< "  :help :h - help (this screen)" << endl
		<< "  :quit :q - quit" << endl << endl
		<< "usage examples:" << endl
		<< "  1+2*3;" << endl
		<< "  3+4*2/(1-5)^2^3;" << endl
		<< endl;
}


void show_about()
{
	cout << "Yet another calculator" << endl
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
	cout << PROMPT;

	char ch;

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
	cout << MOTD;

	while (true) { commandline(); }

	return 0;
}
