#pragma once
#include <string>
#include <vector>
#include "Trie.h"
#include "Trie_Node.h"

using namespace std;

class Trie
{
private:
	Trie_Node* root;

public:
	Trie();

	void find_file_name(string s, vector<string>&);
	void Add_Word(string, string);
	void Delete_Word(Trie_Node*, string);
	void Print_File_Name(Trie_Node* tnode, vector<string>&);
	void Print_File_Name(Trie_Node* tnode);
	void Add_File_Name(Trie_Node*, string);
	void Delete_File_Name(Trie_Node*, string);
	void Build(const string& file_direction, const vector<string>& StopWords, vector<string>&);
	void Add_New_File(const string&, const vector<string>&, vector<string>&);
	void Search(vector<string>&);
	void Delete(vector<string>&);
	void traverse(std::string& prefix, Trie_Node* tnode);
	void Get_List_of_Words();
	void Get_List_of_Files(vector<string>&, const string&);
	void Update(const string&, const vector<string>&);

	int Number_Of_Files(Trie_Node*);
	int Number_Of_Nodes(Trie_Node*);
	int height(Trie_Node* tnode);

	vector<string> open(string path, bool& found);
	Trie_Node* Trie::getroot();

	~Trie();
};
