#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <limits>
#include <cmath>

#define MAX_LENGTH_REF 100
#define MAX_LENGTH 20000
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define TH_PID 80

using namespace std;

float homology(string s, string t);

int main(int argc, char * argv[])
{
	if (argc != 7)
	{
		std::cout << "Usage: new_generation [fsa_file] [undefs_txt] [posneg_txt] [yeast.fasta] [refs_txt] [dognumber_txt] > [_undef.gff]" << endl;
		return -1;
	}

	map<string, string> map_header;
	vector<string> names_head;
	ifstream fin_fsa(argv[1]), fin_undef(argv[2]), fin_posneg(argv[3]);
	string str_undef, buf_undef = "", str_undef_start, str_undef_end, head, sign;
	char line[MAX_LENGTH];
	string line2;
	long final_start = 0, final_end = 0;
	vector<string> undefs;

	map<string, string> map_ref;
	vector<string> names_chr;
	ifstream fin_chr(argv[4]), fin_ref(argv[5]);
	string str_ref, buf_ref = "", chr, str_ref_start, str_ref_end, gene, str_ref_prob, str_evalue, real_name;

	vector<string> refs;
	char * pEnd;
	long ref_start, ref_end, ref_length;
	string ref;
	string str_posneg;

	// map head
	while (fin_fsa.getline(line, MAX_LENGTH - 1))
	{
		str_undef.assign(line);
		if (str_undef.empty())
		{
			map_header.insert(make_pair(names_head.back(), buf_undef));
			break;
		}
		if (str_undef.at(0) == '>')
		{
			string str_sub = str_undef.substr(1, str_undef.find_first_of(' ') - 1);
			if (!names_head.empty())
				map_header.insert(make_pair(names_head.back(), buf_undef));
			names_head.push_back(str_sub);
			buf_undef = "";
		}
		else
		{
			buf_undef += line;
		}
	}

	// map ref
	while (fin_chr.getline(line, MAX_LENGTH_REF - 1))
	{
		str_ref.assign(line);
		if (str_ref.empty())
		{
			map_ref.insert(make_pair(names_chr.back(), buf_ref));
			break;
		}
		if (str_ref.at(0) == '>')
		{
			string str_sub = str_ref.substr(1, str_ref.find_first_of(' ') - 1);
			if (!names_chr.empty())
				map_ref.insert(make_pair(names_chr.back(), buf_ref));
			names_chr.push_back(str_sub);
			buf_ref = "";
		}
		else
		{
			buf_ref += line;
		}
	}
	map_ref.insert(make_pair(names_chr.back(), buf_ref));


	//dognumber
	ifstream fin_dog(argv[6]);
	string str_dog;
	vector<int> dogvector;
	while (fin_dog.getline(line, MAX_LENGTH_REF - 1))
	{
		str_dog.assign(line);
		dogvector.push_back(stoi(str_dog));
	}

	while (fin_undef.getline(line, MAX_LENGTH - 1))
	{
		// reading undef
		str_undef.assign(line);
		size_t pos;
		head = str_undef.substr(0, str_undef.find(' '));
		pos = str_undef.find(' ');
		size_t temp = str_undef.find(' ', pos + 1);
		str_undef_start = str_undef.substr(pos + 1, temp - pos - 1);
		pos = str_undef.find(' ', pos + 1);
		temp = str_undef.find(' ', pos + 1);
		str_undef_end = str_undef.substr(pos + 1, temp - pos - 1);

		if (str_undef_start == "1340" && str_undef_end == "1714")
		{
			std::cout << endl;
		}

		// reading ref
		fin_ref >> gene;
		if (gene == "NONE")
		{
			dogvector.erase(dogvector.begin());
			long undef_start = strtol(str_undef_start.data(), &pEnd, 10);
			long undef_end = strtol(str_undef_end.data(), &pEnd, 10);
			std::cout << head << " agape gene " << undef_start << " " << undef_end << " . " << sign << " . " << "UNDEF" << endl;
			continue;
		}
		fin_ref >> str_ref_prob;
		fin_ref >> str_evalue;
		fin_ref >> real_name;
		fin_ref >> chr;
		fin_ref >> str_ref_start;
		fin_ref >> str_ref_end;

		// find sign
		// fin_posneg.seekg(0, ios::beg);
		while (fin_posneg.getline(line, MAX_LENGTH - 1))
		{
			str_posneg.assign(line);

			size_t pos2;
			string gene2 = str_posneg.substr(0, str_posneg.find(' '));
			pos2 = str_posneg.find(' ');
			size_t temp = str_posneg.find(' ', pos2 + 1);
			string start2 = str_posneg.substr(pos2 + 1, temp - pos2 - 1);
			pos2 = str_posneg.find(' ', pos2 + 1);
			temp = str_posneg.find(' ', pos2 + 1);
			string end2 = str_posneg.substr(pos2 + 1, temp - pos2 - 1);

			if (head.compare(gene2) < 0)
			{
				break;
			}

			if (head == gene2 && str_undef_start == start2 & str_undef_end == end2)
			{
				pos2 = str_posneg.find(' ', pos2 + 1);
				temp = str_posneg.find(' ', pos2 + 1);
				sign = str_posneg.substr(pos2 + 1, temp - pos2 - 1);
				break;
			}
		}

		// bring ref
		ref_start = strtol(str_ref_start.data(), &pEnd, 10);
		ref_end = strtol(str_ref_end.data(), &pEnd, 10);
		double ref_prob = strtod(str_ref_prob.data(), &pEnd);

		if (ref_start > ref_end)
		{
			long swap = ref_end;
			ref_end = ref_start;
			ref_start = swap;
		}

		ref_length = ref_end - ref_start + 1;
		ref = map_ref[chr].substr(ref_start - 1, ref_length);
		refs.push_back(ref);

		//extension
		string extended_undef;
		long origin_start = strtol(str_undef_start.data(), &pEnd, 10);
		long origin_end = strtol(str_undef_end.data(), &pEnd, 10);
		int dog = dogvector.front();
		dogvector.erase(dogvector.begin());
		float pid_cur = homology(map_header[head].substr(origin_start - 1, origin_end - origin_start + 4), ref);

		final_start = origin_start;
		final_end = origin_end;

		if (dog == 1)
		{
			long origin_length = origin_end - origin_start + 1;
			long ref_length = ref_end - ref_start + 1;

			extended_undef = map_header[head].substr(origin_start - 1, origin_end - origin_start + 4);
			float prob = homology(extended_undef, ref);
			if ((origin_length <= ref_length * 0.7 || origin_length >= ref_length) && prob >= 85.0)
			{
				std::cout << head << " agape gene " << final_start << " " << final_end << " . " << sign << " . " << gene << ",SacCer," << chr << "," << ref_start << "," << ref_end << "," << real_name << "," << str_evalue << "," << prob << endl;
			}
			else
			{
				std::cout << head << " agape gene " << final_start << " " << final_end << " . " << sign << " . " << "UNDEF" << endl;
			}
		}
		else if (dog >= 2)
		{
			// 엑손 두개 가져와서 합침
			char line2[MAX_LENGTH];
			for (int i = 0; i < dog - 1; i++)
			{
				fin_undef.getline(line2, MAX_LENGTH - 1);
				string str_undef2;
				str_undef2.assign(line2);

				size_t pos2;
				string head2 = str_undef2.substr(0, str_undef2.find(' '));
				pos2 = str_undef2.find(' ');
				size_t temp2 = str_undef2.find(' ', pos2 + 1);
				string str_undef2_start = str_undef2.substr(pos2 + 1, temp2 - pos2 - 1);
				pos2 = str_undef2.find(' ', pos2 + 1);
				temp2 = str_undef2.find(' ', pos2 + 1);
				string str_undef2_end = str_undef2.substr(pos2 + 1, temp2 - pos2 - 1);

				long origin2_start = strtol(str_undef2_start.data(), &pEnd, 10);
				long origin2_end = strtol(str_undef2_end.data(), &pEnd, 10);
				final_end = origin2_end;

				extended_undef = map_header[head].substr(origin_start - 1, origin_end - origin_start + 4) + map_header[head].substr(origin2_start - 1, origin2_end - origin2_start + 4);
			}
			float homo_temp = homology(extended_undef, ref);
			if (homo_temp >= 80)
				std::cout << head << " agape gene " << final_start << " " << final_end << " . " << sign << " . " << gene << ",SacCer," << chr << "," << ref_start << "," << ref_end << "," << real_name << "," << str_evalue << "," << homo_temp << endl;
			else
				std::cout << head << " agape gene " << final_start << " " << final_end << " . " << sign << " . " << "UNDEF" << endl;
		}
	}
	return 0;
}

float homology(string s, string t)
{
	s = '0' + s;
	t = '0' + t;
	int len1 = s.size();
	int len2 = t.size();
	int max_size = len1 > len2 ? len1 : len2;
	int min_size = len1 > len2 ? len2 : len1;
	int **lcs = new int *[len1];
	for (int i = 0; i < len1; i++)
		lcs[i] = new int[len2];
	for (int i = 0; i < len1; i++)
	{
		for (int j = 0; j < len2; j++)
		{
			if (i == 0 || j == 0)
			{
				lcs[i][j] = 0;
				continue;
			}
			if (s[i] == t[j])
				lcs[i][j] = lcs[i - 1][j - 1] + 1;
			else
			{
				if (lcs[i - 1][j] > lcs[i][j - 1])
					lcs[i][j] = lcs[i - 1][j];
				else
					lcs[i][j] = lcs[i][j - 1];
			}
		}
	}
	float result = lcs[len1 - 1][len2 - 1] / static_cast<float>(min_size - 1) * 100;
	return result;
}
