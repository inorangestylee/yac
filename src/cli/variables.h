#pragma once

#include <iostream>
#include <vector>

namespace calculator {
	using namespace std;

	class Variable
	{
	public:
		string name;
		double value;

		Variable(string n, double v) : name(n), value(v) {};
	};

	class VariableTable
	{
	public:
		double get(string n);
		void set(string n, double v);
		bool is_present(string n);

		VariableTable();
	private:
		vector<Variable> data;
	};
}
