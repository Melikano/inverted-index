#include<iostream>
#include<fstream>
#include<string>
#include<vector>

using namespace std;


void GetStopWords(vector<string>& StopWords) // Reading StopWords
{
	string StopWord{};
	ifstream fin{"StopWords.txt"};

	if (!fin)
	{
		cout << " Error : Stop Words was not found " << endl;
		return;
	}

	do {
		fin >> StopWord;
		StopWords.push_back(StopWord);

	} while (!fin.eof());

	return;
}

//////////////////////////////////////////// Reading input files ///////////////////////////////////////////////

void AddNewFile(const string& file_direction, string& file_name, vector<string>& words, const vector<string>& StopWords, int& File_counter, bool& found)
{
	
	bool IsStopWord {false};

	string word{};
	string bottom{ 'a' };
	string up{ 'z' };

	bool file_found{ true };

	ifstream fin{};
	string Newfile{ file_direction + '\\' + file_name };

	fin.open(Newfile);

	if (!fin)
	{
		cout << "file wasn't found" << endl;
		found = false;
		return;
	}

	do {
		fin >> word;
		for (auto& t : word)
			t = static_cast<char>(tolower(static_cast<int>(t)));

		IsStopWord = false;
		for (size_t i{}; i < StopWords.size(); i++)
		{
			if (word == StopWords[i])
			{
				IsStopWord = true;
				i = StopWords.size() + 1;
			}
		}
		if (!IsStopWord && bottom < word && word < up)
			words.push_back(word);

	} while ( !fin.eof());

	File_counter++;

	fin.close();

}

