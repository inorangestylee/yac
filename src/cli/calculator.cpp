#include "calculator.h"

#include <cmath>
#include <deque>
#include <iostream>
#include <vector>

#include "error.h"
#include "math_ex.h"
#include "variables.h"

namespace calculator {

	using namespace std;

	enum class TokenType {
		EMPTY,
		NUM,
		OP,
		FUNC,
		VAR,
		SEP,
		L_BR,
		R_BR,
	};

	class Token {
	public:

		TokenType type;
		double value;
		char ch;
		int priority;
		bool right_assoc;
		string name;

		Token(TokenType t, double v, char c = '0', int p = -1, bool ra = false, string n = "")
			: type(t), value(v), ch(c), priority(p), right_assoc(ra), name(n) {}
	};

	deque<Token> getTokens() {
		deque<Token> tokens;
		char ch;
		while (cin >> ch) {
			if (isdigit(ch)) {
				cin.putback(ch);
				double v;
				cin >> v;
				auto token = Token(TokenType::NUM, v);
				tokens.push_back(token);
			} 
			else if (isalpha(ch)) {
				cin.putback(ch);
				char temp_ch;
				string name = "";
				while (cin >> temp_ch && isalpha(temp_ch)) {
					name += temp_ch;
				}
				// предполагается что переменные будут в UPPERCASE (A, B, C, PI)
				// функции в lowercase (log, exp, ln, mod)			
				auto token = (isupper(ch))
					? Token(TokenType::VAR, 0, '0', -1, false, name)
					: Token(TokenType::FUNC, 0, '0', -1, false, name);
				
				double args_count = 1;

				if (token.type == TokenType::FUNC && token.name == "logn")
				{
					args_count = 2;
				}
				
				token.value = args_count;

				tokens.push_back(token);
				cin.putback(temp_ch);
			}
			else {
				TokenType type = TokenType::EMPTY;
				int priority = -1;
				bool right_assoc = false;

				switch (ch) {
				case ',':
					type = TokenType::SEP;
					break;
				case '(':
				{
					type = TokenType::L_BR;
					break;
				}
				case ')':
				{
					type = TokenType::R_BR;
					break;
				}
				case '^':
				{
					type = TokenType::OP;
					priority = 4;
					right_assoc = true;
					break;
				}
				case '*': case '/': case '%':
				{
					type = TokenType::OP;
					priority = 3;
					break;
				}
				case '+': case '-':
				{
					type = TokenType::OP;
					priority = 2;
					break;
				}
				case ';':
					return tokens;
				}
				auto token = Token(type, 0, ch, priority, right_assoc);
				tokens.push_back(token);
			}
		}
	};

	deque<Token> getRPN(deque<Token> tokens)
	{
		deque<Token> output;
		vector<Token> stack;

		for (auto t : tokens) {
			switch (t.type) {
			{
			case TokenType::NUM: case TokenType::VAR:
				output.push_back(t);
				break;
			}
			
			case TokenType::FUNC:
				stack.push_back(t);
				break;

			case TokenType::SEP:
			{
				// ПОКА на вершине стэка не открывающая скобка {
				//   Переложить op из стэка в выходную очередь
				//   ЕСЛИ стэк пустой - швыряем фаталку (пропущена запятая или скобка)
				// }
				while (stack.back().type != TokenType::L_BR)
				{
					output.push_back(stack.back());
					stack.pop_back();
					if (stack.empty()) {
						Fatal("separator or parenthesis mismatch");
					}
				}
				break;
			}

			case TokenType::OP:
			{
				const Token o1 = t;

				while(!stack.empty()) {
					const Token o2 = stack.back();
					// ЕСЛИ ( o1 лево-ассоциативный И его приоритет <= o2
					// ИЛИ
					// o1 право-ассоциативный И его приоритет < o2 ) {
					//   ТО переещаем o2 из стэка в очередь вывода
					// }
					// Добавляем o1 в стэк
					if (
						(!o1.right_assoc && o1.priority <= o2.priority) ||
						(o1.right_assoc && o1.priority < o2.priority)
						) {
						output.push_back(o2);
						stack.pop_back();
						continue;
					}
					break;
				}
				stack.push_back(o1);
				break;
			}

			case TokenType::L_BR:
				stack.push_back(t);
				break;

			case TokenType::R_BR:
			{
				bool is_ok = false;
				// WHILE оператор на верщине стэка не открывающая скобка {
				//   ПРОВЕРЯЕМ что стэк не пустой {
				//     ЕСЛИ стэк пустой -> швыряем фаталку (скобка не найдена)
				//   }
				//   ПЕРЕМЕЩАЕМ оператор из стэка в очередь вывода
				// }
				// УНИЧТОЖАЕМ открывающую скобку
				// ЕСЛИ на вершине стэка функция
				//   ПЕРЕМЕЩАЕМ функцию в очередь вывода

				while (!stack.empty() && stack.back().type != TokenType::L_BR) {
					output.push_back(stack.back());
					stack.pop_back();
					is_ok = true;
				}
				stack.pop_back();
				
				if (!is_ok && stack.empty()) {
					Fatal("parenthesis mismatch");
				}

				if (!stack.empty() && stack.back().type == TokenType::FUNC) {
					output.push_back(stack.back());
					stack.pop_back();
				}
				break;
			}
			default: 
				Fatal("unknown token");
			}
		}
		// после окончания токенов переносим стэк в очередь вывода
		while (!stack.empty()) {
			// если открывающая скобка - швыряем фаталку
			if (stack.back().type == TokenType::L_BR) {
				Fatal("parenthesis mismatch");
			}
			output.push_back(stack.back());
			stack.pop_back();
		}

		return output;
	}

	deque<Token> evalRPN(deque<Token> tokens)
	{
		auto vt = VariableTable::VariableTable();
		
		while (tokens.size() > 1) {
			auto t = tokens.front();

			switch (t.type) {
			case TokenType::NUM: {
				tokens.push_back(t);
				tokens.pop_front();
				break;
			}
			case TokenType::VAR:
			{
				double v = vt.get(t.name);
				auto num_token = Token(TokenType::NUM, v);
				tokens.push_back(num_token);
				tokens.pop_front();
				break;
			}
			case TokenType::OP:
			{
				double b = tokens.back().value;
				tokens.pop_back();

				double a = tokens.back().value;
				tokens.pop_back();

				double r;
				if		(t.ch == '+') { r = a + b; }
				else if (t.ch == '-') { r = a - b; }
				else if (t.ch == '*') { r = a * b; }
				else if (t.ch == '/') { r = division(a, b); }
				else if (t.ch == '%') { r = division(a, b, true); }
				else if (t.ch == '^') { r = pow(a, b); }
				else {
					Fatal("unknown operation");
				}
				tokens.pop_front();
				tokens.push_back(Token(TokenType::NUM, r));
				break;
			}
			case TokenType::FUNC:
			{
				vector<double> args;
				for (int i = 0; i < (int)t.value; ++i)
				{
					args.push_back(tokens.back().value);
					tokens.pop_back();
				}

				double r;
				if (t.name == "lg")			{ r = log10(args[0]); }
				if (t.name == "log")		{ r = log2(args[0]); }
				else if (t.name == "ln")	{ r = log(args[0]); }
				else if (t.name == "sqrt")	{ r = sqrt(args[0]); }
				else if (t.name == "fact")	{ r = factorial((unsigned int)args[0]); } //?
				else if (t.name == "abs")	{ r = abs(args[0]); }
				else if (t.name == "exp")	{ r = exp(args[0]); }
				else {
					string err_msg = "unknown function '" + t.name + "'";
					Fatal(err_msg);
				}
				break;
			}
			default:
				Fatal("unknown token type");
			}
		}
		return tokens;
	}



	void calculate()
	{
		auto tokens = getTokens();
		auto queue = getRPN(tokens);
		auto resultq = evalRPN(queue);

		cout << "reverse polish notation:";
		for (auto t : queue) {
			cout << ' ';
			switch (t.type) {
			case TokenType::NUM:
				cout << t.value;
				break;
			case TokenType::FUNC: case TokenType::VAR:
				cout << t.name;
				break;
			default:
				cout << t.ch;
				break;
			}
		}
		cout << endl;
		cout << "result: " << resultq[0].value << endl;
	}
}
