#pragma once
#include <string>
#include <vector>
#include "Hash_Node.h"

using namespace std;

class Hash
{
private:
	Hash_Node **htable;

public:
	Hash();

	int HashFunc(int key);
	int Number_Of_Files(Hash_Node* hnode);
	int Number_Of_Words();

	void Add_File_Name(Hash_Node* hnode, string FileName);
	void Delete_File_Name(Hash_Node* hnode, string FileName);
	void Add_Word(string NewWord, string FileName);
	void Delete_Word(string FileName);
	void find_file_name(string SearchedWord, vector<string>& History);
	void Print_File_Name(Hash_Node* hnode, vector<string>& History);
	void Print_File_Name(Hash_Node* bnode);
	void Build(const string& file_direction, const vector<string>& StopWords, vector<string>& files);
	void Add_New_File(const string&, const vector<string>&, vector<string>&);
	void Search(vector<string>&);
	void Delete(vector<string>&);
	void Get_List_of_Words();
	void Update(const string&, const vector<string>&);
	void Get_List_of_Files(vector<string>&, const string&);
	void traverse();

	vector<string> Hash::open(string path, bool& found);

	~Hash();
};

