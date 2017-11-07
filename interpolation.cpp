#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#define MAX_LENGTH 10000

using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: temp2.sh [blastx.out file] [gff file]" << endl;
		return -1;
	}
	ifstream fin_blastx(argv[1]);
	ifstream fin_gff(argv[2]);

	int lineno = 0;

	ofstream fout_undefs("undefs.txt");
	ofstream fout_refs_tmp("refs_tmp.txt");

	bool cond1;
	size_t cond2, cond4;
	std::regex reg("^Y|^Q");
	std::smatch m;

	bool count = 0;

	char buf_blastx[MAX_LENGTH];
	string str_blastx;
	while (fin_blastx.getline(buf_blastx, MAX_LENGTH - 1))
	{
		//std::cout << ++lineno << endl;

		str_blastx.assign(buf_blastx);
		
		if (str_blastx.empty())
			continue;

		cond1 = str_blastx.at(0) == '#';
		if (cond1 == true)
		{
			cond2 = str_blastx.find("Query");
			if (cond2 != string::npos)
			{
				fout_undefs << str_blastx << endl;

				count = 0;
				continue;
			}
			cond4 = str_blastx.find("# 0 hits");
			if (cond4 != string::npos)
			{
				fout_refs_tmp << "NONE" << endl;
				count = 1;
				continue;
			}
			if (cond2 == string::npos || cond4 == string::npos)
				continue;
		}
		if(std::regex_search(str_blastx, m, reg))
		{
			if (count == 0)
			{
				fout_refs_tmp << str_blastx << endl;
				count = 1;
			}
		}
	}
	return 0;

}

