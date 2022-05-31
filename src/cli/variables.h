#pragma once

#include <iostream>
#include <vector>


namespace calculator {

	class Variable
	{
	public:
		std::string name;
		double value;

		Variable(std::string n, double v) : name(n), value(v) {};
	};

	class VariableTable
	{
	public:
		double get(std::string n);
		void set(std::string n, double v);
		bool is_present(std::string n);

		VariableTable();
	private:
		std::vector<Variable> data;
	};
}
