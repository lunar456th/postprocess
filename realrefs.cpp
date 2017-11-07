#include <iostream>
#include <fstream>

#define MAX_LENGTH 100

using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 4)
	{
		std::cout << "Usage: realrefs [_undef.gff] [undefs.txt] [refs.txt] > [real_refs.txt]" << endl;
		return -1;
	}

	ifstream fin_gff(argv[1]);
	ifstream fin_undef(argv[2]);
	ifstream fin_ref(argv[3]);

	char buf_gff[MAX_LENGTH], buf_undef[MAX_LENGTH], buf_ref[MAX_LENGTH];
	string str_gff, str_undef, str_ref;
	while (fin_gff.getline(buf_gff, MAX_LENGTH - 1))
	{
		str_gff.assign(buf_gff);

		while (str_gff != str_undef)
		{
			if (!fin_undef.getline(buf_undef, MAX_LENGTH - 1))
			{
				return 0;
			}
			fin_ref.getline(buf_ref, MAX_LENGTH - 1);
			str_undef.assign(buf_undef);
		}
		str_ref.assign(buf_ref);
		std::cout << str_ref.data() << endl;
	}

	return 0;
}