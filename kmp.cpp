#include <iostream>
#include <string>

using namespace std;

class Pat
{
private:
	int* ff;
	string pat;
	int len;
public:
	Pat(const string& str);
	~Pat();
	void showFF();
	bool match(string& str);
};



int main(void)
{
	string str = "ababaabaadcd";
	Pat pat("abaad");
	pat.showFF();
	pat.match(str);
	string str2 = "sdkfjlkwfwfdfejkjfhrtieroogpjefoerjpgkn3hrjkwfjkjkjldkfjalskjglkgljlkerjlktlkmrlkfemlkdajfg";
	Pat pat2("ajfg");
	pat2.showFF();
	pat2.match(str2);
	return 0;
}



//Function
Pat::Pat(const string& str):len(str.length()), pat(str)
{
	ff = new int[len];
	int k = 0, j = 0;
	ff[0] = 0;
	for (int i = 1; i < len; i++)
	{
		k = ff[i - 1];
		if (pat[k] == pat[i])
			ff[i] = k + 1;
		else
		{
			if (pat[i] == pat[0])
				ff[i] = 1;
			else
				ff[i] = 0;
		}
	}
}

Pat::~Pat()
{
	delete ff;
	pat.~string();
}

void Pat::showFF()
{
	cout << pat << "의 FailureFunction: ";
	for (int i = 0; i < len; i++)
		cout << ff[i] << ' ';
	cout << endl;
}

bool Pat::match(string& str)
{
	int idx = 0; //ff값을 읽어오기위한.
	int m = str.length();
	for (int i = 0; i < m; i++)
	{
		while (idx > 0 && pat[idx] != str[i])
			idx = ff[idx - 1];
		if (str[i] == pat[idx])
			idx++;
		if (idx == len)
		{
			cout << "str[" << i - len + 1 << "-" << i << "] match found" << endl;
			return true;
		}	
	}
	cout << "Not found" << endl;
	return false;
}