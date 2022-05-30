#include <iostream>
#include <vector>
#include <deque>

namespace calculator {

	using namespace std;

	enum class TokenType {
		EMPTY,
		NUM,
		OP,
		L_BR,
		R_BR
	};

	class Token {
	public:
		
		TokenType type;
		double value;
		char ch;
		int priority;

		Token(TokenType t, double v, char c = '0', int p = -1)
			: type(t), value(v), ch(c), priority(p) {}
	};

	/// <summary>
	/// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
	/// </summary>
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
			else {
				TokenType _type = TokenType::EMPTY;
				int _priority = -1;
				
				switch (ch) {
				case '(':
					_type = TokenType::L_BR;
					break;
				case ')':
					_type = TokenType::R_BR;
					break;
				case '^':
					_type = TokenType::OP;
					_priority = 4;
					break;
				case '*': case '/':
					_type = TokenType::OP;
					_priority = 3;
					break;
				case '+': case '-':
					_type = TokenType::OP;
					_priority = 2;
					break;
				}
				auto _token = Token(_type, 0, ch, _priority);
				tokens.push_back(_token);
			}
		}
		return tokens;
	};


	void calculate()
	{
		auto tokens = getTokens();
	}
}
