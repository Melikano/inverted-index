#pragma once
#include <string>
#include <vector>
#include "BST_Node.h"

using namespace std;

class BST
{
private:
	BST_Node* root {nullptr};

public:
	BST();

	int height(BST_Node*);
	int Balance_Factor(BST_Node*);
	int Number_Of_Nodes(BST_Node*);
	int Number_Of_Files(BST_Node*);
	
	BST_Node* RR_Rotation(BST_Node*);
	BST_Node* LL_Rotation(BST_Node*);
	BST_Node* LR_Rotation(BST_Node*);
	BST_Node* RL_Rotation(BST_Node*);
	BST_Node* Make_Balance(BST_Node*);
	BST_Node* Add_Word(BST_Node*, string, string);
	BST_Node* Min_Node(BST_Node*);
	BST_Node* Max_Node(BST_Node*);
	BST_Node* find_file_name(string SearchedWord, BST_Node* bnode, vector<string>&);
	BST_Node* getroot();

	void Delete_Word(BST_Node*, string);
	void Add_Word(string, string);
	void Delete_Word(string);
	void Add_File_Name(BST_Node*, string);
	void Delete_File_Name(BST_Node*, string);
	void FindFileName(string SearchedWord, vector<string>&);
	void inorder(BST_Node *tree);
	void Print_File_Name(BST_Node* bnode,vector<string>&);
	void Print_File_Name(BST_Node* bnode);
	void Build(const string&, const vector<string>&, vector<string>&);
	void Add_New_File(const string&, const vector<string>&, vector<string>& );
	void Search(vector<string>&);
	void Delete(vector<string>&);
	void Get_List_of_Words();
	void Update(const string& , const vector<string>& );
	void Get_List_of_Files(vector<string>& , const string& );

	vector<string> open(string path, bool& found);
	
	~BST();
};

