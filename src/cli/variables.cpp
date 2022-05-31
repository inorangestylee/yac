#include "variables.h"

#include <iostream>
#include <vector>

#include "error.h"


namespace calculator
{
    using namespace std;

    VariableTable::VariableTable() {
        data = {
            Variable("PI", 3.141592),
            Variable("E", 2.718281)
        };
    }


    bool VariableTable::is_present(string n)
    {
        for (const Variable& v : data)
        {
            if (v.name == n) { return true; }
        }
        return false;
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
}
