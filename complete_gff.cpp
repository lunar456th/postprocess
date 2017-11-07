#include <iostream>
#include <fstream>

#define MAX_LENGTH 10000

using namespace std;

int main(int argc, char * argv[])
{

	if (argc != 3)
	{
		std::cout << "Usage: complete_gff [original .gff file] [post-processed .gff file] > [output file]" << endl;
		return 0;
	}

	ifstream fin_gff(argv[1]);
	ifstream fin_undef(argv[2]);

	char buf_gff[MAX_LENGTH];
	char buf_undef[MAX_LENGTH];
	string str_gff;
	string str_undef;
	size_t isUndef;

	fin_undef.getline(buf_undef, MAX_LENGTH - 1);
	str_undef.assign(buf_undef);
	while (fin_gff.getline(buf_gff, MAX_LENGTH - 1))
	{

		str_gff.assign(buf_gff);

		if (str_gff.find("UNDEF") != string::npos)
		{
			std::cout << str_undef.data() << endl;
			fin_undef.getline(buf_undef, MAX_LENGTH - 1);
			str_undef.assign(buf_undef);
		}
		else
		{
			std::cout << str_gff.data() << endl;
		}

	}

	return 0;
}