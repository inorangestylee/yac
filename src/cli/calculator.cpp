#include <iostream>
#include <vector>
#include <deque>
#include "error.h"

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
				double _v;
				cin >> _v;
				auto _token = Token(TokenType::NUM, _v);
				tokens.push_back(_token);
			} 
			else if (isalpha(ch)) {
				cin.putback(ch);
				char _ch;
				string _name = "";
				while (cin >> _ch && isalpha(_ch)) {
					_name += _ch;
				}
				// предполагается что переменные будут в UPPERCASE (A, B, C, PI)
				// функции в lowercase (log, exp, ln, mod)
				auto _token = (isupper(ch))
					? Token(TokenType::VAR, 0, '0', -1, false, _name)
					: Token(TokenType::FUNC, 0, '0', -1, false, _name);
				tokens.push_back(_token);
				cin.putback(_ch);
			}
			else {
				TokenType _type = TokenType::EMPTY;
				int _priority = -1;
				bool _right_assoc = false;

				switch (ch) {
				case ',':
					_type = TokenType::SEP;
					break;
				case '(':
				{
					_type = TokenType::L_BR;
					break;
				}
				case ')':
				{
					_type = TokenType::R_BR;
					break;
				}
				case '^':
				{
					_type = TokenType::OP;
					_priority = 4;
					_right_assoc = true;
					break;
				}
				case '*': case '/': case '%':
				{
					_type = TokenType::OP;
					_priority = 3;
					break;
				}
				case '+': case '-':
				{
					_type = TokenType::OP;
					_priority = 2;
					break;
				}
				case ';':
					return tokens;
				}
				auto _token = Token(_type, 0, ch, _priority, _right_assoc);
				tokens.push_back(_token);
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
				bool _ok = false;
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
					_ok = true;
				}
				stack.pop_back();
				
				if (!_ok && stack.empty()) {
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
		vector<string> trace;
		int step = 0;
		while (tokens.size() > 1) {
			++step;
			for (int i = 0; i < tokens.size(); ++i)
			{
				if (tokens[i].type == TokenType::OP) {
					double _r;
					switch (tokens[i].ch)
					{
					case '*':
						//_r = tokens[i - 2].value * tokens[i - 1].value;
						//for (deque<Token>::iterator it = tokens.at(i-2); it != tokens(i); )
						//{
						//	tokens.
						//	tokens.erase(it);
						//	++it;
						//}
						//	tokens.pop_back(i - 2);
						//break;
					}
				}
			}
		}
	}



	void calculate()
	{
		auto tokens = getTokens();
		auto queue = getRPN(tokens);
		
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
	}
}
