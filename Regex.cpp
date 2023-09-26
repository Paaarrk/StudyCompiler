#include <iostream>
#include <regex>
#include <vector>
#include <string>
#pragma warning (disable : 4996)

using namespace std;

int main(void)
{
	vector<string> phone_nums = { "010-8833-9484", "02-343-3243", "120-3423-3432", "010-2344-343" };

	regex checkPhonenum("0[0-9]{1,2}[-][0-9]{3,4}[-][0-9]{4}");

	for (const auto phone_num : phone_nums)
	{
		cout << phone_num << ": " << regex_match(phone_num, checkPhonenum) << endl;
	}
	return 0;
}