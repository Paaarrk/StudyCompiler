#include <iostream>
#include <string>
#include <array>
#pragma warning (disable : 4996)

using namespace std;

static const string T_EXPR = "expr";
static const string T_IF = "if";
static const string T_WHILE = "while";
static const string T_FOR = "for";
static const string T_OTHER = "other";

static string lookAhead = "";
static int index = 0;



/* Code for parsing */
static array<string, 5> code = { T_WHILE, "(", T_EXPR, ")", T_OTHER };
//static array<string, 3> code = { T_FOR, "(", ";" }; - Error Code


static void report(string msg)
{
	cout << msg << endl;
	exit(1);
}

static void match(string t)
{
	if (lookAhead._Equal(t))
		lookAhead = ++index < code.size() ? code[index] : "";
	else
		report("syntax error");
}

static void optexpr()
{
	if (lookAhead._Equal(T_EXPR))
		match(T_EXPR);
}

static void stmt()
{
	if (lookAhead._Equal(T_EXPR))
	{
		match(T_EXPR); match(";");
	}
	else if (lookAhead._Equal(T_IF))
	{
		match(T_IF); match("("); match(T_EXPR); match(")"); stmt();
	}
	else if (lookAhead._Equal(T_WHILE))
	{
		match(T_WHILE); match("("); match("expr"); match(")"); stmt();
	}
	else if (lookAhead._Equal(T_FOR))
	{
		match(T_FOR); match("(");
		optexpr(); match(";"); optexpr(); match(";"); optexpr();
		match(")"); stmt();
	}
	else if (lookAhead._Equal(T_OTHER))
	{
		match(T_OTHER);
	}
	else {
		//defalult
		report("Syntax Error!");
	}
}

int main()
{
	lookAhead = code[index];
	stmt();
	cout << "Parsing OK!" << endl;
}