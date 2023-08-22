#include <iostream>
#include <cstring>

using namespace std;

const int MAX = 80;
const int LEN = 40;

class Token
{
public:
	virtual double getNumber() = 0;
	virtual char getOperator() = 0;
};

class Operator : public Token
{
	char oper;
public:
	Operator(char op) { oper = op; }
	char getOperator() { return oper; }
	double getNumber() { return 0; };
};

class Number : public Token
{
	double number;
public:
	Number(double num) { number = num; }
	double getNumber() { return number; }
	char getOperator() { return '0'; }
};

class Stack
{
	Token* st[MAX];
	int top;
public:
	Stack() 
	{ 
		top = 0;
		for (int i = 0; i < MAX; i++)
		{
			st[i] = NULL;
		}
	}
	void push(Token* a)
	{
		st[++top] = a;
	}
	Token* pop()
	{
		return st[top--];
	}
	int gettop()
	{
		return top;
	}

};

class express
{
	Stack s;
	char* str;
	size_t len;
public:
	express(char *ptr)
	{
		len = strlen(ptr);
		str = ptr;
	}
	void parse()
	{
		char ch;
		double lastval = 0;
		double valpop = 0;
		bool dofrac = false;
		int lenght = 1;
		char lastop,prelastop;

		for (int j = 0; j < len; j++)
		{
			ch = str[j];


			if (((ch >= '0') && (ch <= '9')) || (ch == '.'))
			{
				if ((ch >= '0') && (ch <= '9'))
				{
					if (dofrac == false)
					{
						lastval = lastval * 10 + (ch - 48);
					}
					else
					{
						lastval = lastval + (static_cast<double>(ch - 48) / pow(10,lenght));
						lenght++;
					}
				}
				else
				{
					dofrac = true;
				}
			}
			else if (ch == '+' || ch == '-' || ch == '/' || ch == '*')
			{
				lenght = 1;
				dofrac = false;
				if (s.gettop() == 0)
				{
					s.push(new Number(lastval));
					s.push(new Operator(ch));
				}
				else 
				{
					lastop = s.pop()->getOperator();
					valpop = s.pop()->getNumber();
					if ((ch == '*' || ch == '/') && (lastop == '-' || lastop == '+'))
					{
						s.push(new Number(valpop));
						s.push(new Operator(lastop));
						s.push(new Number(lastval));
						s.push(new Operator(ch));
					}
					else
					{
						if (s.gettop() != 0)
						{
							prelastop = s.pop()->getOperator();
							if (prelastop == '-')
							{
								valpop *= -1;
								s.push(new Operator('+'));
							}
							else
							{
								s.push(new Operator(prelastop));
							}
						}
						switch (lastop)
						{
						case'+':s.push(new Number(valpop + lastval)); break;
						case'-':s.push(new Number(valpop - lastval)); break;
						case'/':s.push(new Number(valpop / lastval)); break;
						case'*':s.push(new Number(valpop * lastval)); break;
						default: cout << "\nНеизвестный оператор"; exit(1);
						}
						s.push(new Operator(ch));
					}
				}
				lastval = 0;
			}
			else
			{
				cout << "\nНеизвестный оператор";
				exit(1);
			}
			if (j == (len - 1))
			{
				s.push(new Number(lastval));
			}
		}
	}
	double solve()
	{
		double lastvalue, prelastvalue;
		char oper, preoper;
		while (s.gettop() > 1)
		{
			lastvalue = s.pop()->getNumber();
			oper = s.pop()->getOperator();
			prelastvalue = s.pop()->getNumber();
			if (s.gettop() != 0)
			{
				preoper = s.pop()->getOperator();
				if (preoper == '-')
				{
					s.push(new Operator('+'));
					prelastvalue *= -1;
				}
				else
				{
					s.push(new Operator(preoper));
				}
			}
			switch (oper)
			{
			case'+':s.push(new Number(prelastvalue + lastvalue)); break;
			case'-':s.push(new Number(prelastvalue - lastvalue)); break;
			case'*':s.push(new Number(prelastvalue * lastvalue)); break;
			case'/':s.push(new Number(prelastvalue / lastvalue)); break;
			default: cout << "\nНеизвестный оператор"; exit(1);
			}
		}
		return (s.pop()->getNumber());
	}
};

double roundNum(double num)
{
	int div = static_cast<int>(num);
	double frac = num - div;
	if (frac > 0.49)
		return (static_cast<int>(num) + 1);
	else
		return static_cast<int>(num);
}
int main()	//	3.14+2.4256-674.999/24.65+7.654321 = 5,5656 - 27,38332657200811 + 7,654321 = -21,81772657200811 + 7,654321 = -14,16340557200811
{
	setlocale(LC_ALL, "rus");
	char ans;
	char str[LEN];
	cout << "\nВведите арифметическое выражение вида 3.14+2.4256-674.999/24.65+7.654321 \nВыражение должно быть без скобок: ";
	do
	{
		cout << "\nВыражение:";
		cin >> str;
		express* eptr = new express(str);
		eptr->parse();
		double answer = eptr->solve();
		cout << "\nРезультат: " << answer;
		cout << "\nРезультат с округлением: " << roundNum(answer);
		delete eptr;
		cout << "\nЕще одно выражение?(y/n): "; cin >> ans;
	} while (ans == 'y');
	return 0;
}