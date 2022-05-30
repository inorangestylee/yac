#include <iostream>
#include <vector>
#include "error.h"

namespace calculator
{
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

	VariableTable::VariableTable() {
		data = {
			Variable("PI", 3.141592),
			Variable("E", 2.718281)
		};
	}

	double VariableTable::get(string n)
	{
		if (is_present(n)) {
			for (const Variable& v : data)
			{
				if (v.name == n)
					return v.value;
			}
		}
		string err_msg = "variable '" + n + "' is not defined";
		Fatal(err_msg);
	}

	bool VariableTable::is_present(string n)
	{
		for (const Variable& v : data)
		{
			if (v.name == n) { return true; }
		}
		return false;
	}
}
