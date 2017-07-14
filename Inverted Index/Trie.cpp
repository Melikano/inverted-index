#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include "StoringData.h"
#include "Trie.h"
#include "Trie_Node.h"
#include "StoringData.h"


using namespace std;

int maximum(int a, int b);
int Trie_File_counter{};

Trie::Trie()
{
	root = new Trie_Node(); // the constructor will creat the root
}

////////////////////////////////////////////// Functions For Adding & Deleteing Files //////////////////////////////////

void Trie::Add_File_Name(Trie_Node* Tnode, string FileName)
{
	// make a new node in files ink list
	Trie_Node* NewFileName = new Trie_Node;
	NewFileName->file_name = FileName;

	// find the right place for the new node
	while (Tnode->link)
	{
		Tnode = Tnode->link;

		if (Tnode->file_name == FileName)
			return;
	}

	Tnode->link = NewFileName;
}

void Trie::Delete_File_Name(Trie_Node* Tnode, string FileName)
{
	Trie_Node* CurrentNode{ Tnode->link };
	Trie_Node* PreviousNode{ nullptr };

	int counter{};

	// finding the desired node and delete it
	while (CurrentNode)
	{
		if (CurrentNode->file_name == FileName)
		{
			if (PreviousNode)
				PreviousNode->link = CurrentNode->link;
			else
				Tnode->link = CurrentNode->link;
			delete CurrentNode;
			return;
		}

		PreviousNode = CurrentNode;
		CurrentNode = CurrentNode->link;
	}
}

int Trie::Number_Of_Files(Trie_Node* Tnode)
{
	if (!Tnode)
		return 0;
	return 1 + Number_Of_Files(Tnode->link);
}


//////////////////////////////////////// Functions For Adding & Deleting Word /////////////////////////////////////////


void Trie::Add_Word(string NewWord, string FileName)
{
	Trie_Node* current = root;

	for (int i = 0; i < NewWord.length(); i++)
	{
		// we go to the child wich has the character of our word
		Trie_Node* child = current->findChild(NewWord[i]); 
		if (child != NULL)
			current = child;

		// if none of children contain the char we creat a new child
		else
		{
			Trie_Node* tmp = new Trie_Node();
			tmp->data = NewWord[i];
			current->children.push_back(tmp);
			current = tmp;
		}
		// if we reach the end of word
		if (i == NewWord.length() - 1)
		{
			current->IsEndOfWord = true;
			Add_File_Name(current, FileName);
		}
	}
}

void Trie::Delete_Word(Trie_Node* tnode, string FileName)
{
	static bool IsDeleted = false;

	if (!tnode)
		return;

	for (int i{}; i != tnode->children.size(); i++)
	{
		// delete the required file from all node's link list recursively 
		Delete_Word(tnode->children[i], FileName);

		Trie_Node* tmp = tnode->children[i];

		if (tmp)
			if (tmp->IsEndOfWord) 
			{
				Delete_File_Name(tmp, FileName);
				 // if a node belongs to that file only, delete it
				if (Number_Of_Files(tmp->link) == 0)
				{
					tnode->children[i] = NULL;
					delete tmp;
				}
			}
	}
}

/////////////////////////////////////////// Functions For Search //////////////////////////////////////////////

void Trie::find_file_name(string SearchedWord, vector<string>& History)
{
	Trie_Node* current = root;

	while (current != NULL)
	{
		for (int i = 0; i < SearchedWord.length(); i++)
		{
			Trie_Node* tmp = current->findChild(SearchedWord[i]);

			if (tmp == NULL) // not found
			{
				cout << "not found" << endl;
				History.push_back("not found");
				return;
			}

			current = tmp;
		}

		if (current->IsEndOfWord)
		{
			cout << "found" << endl;
			Print_File_Name(current,History);
			return;
		}

		else //chars found but dont creat a word
		{
			cout << "not found" << endl; 
			return;
		}
	}
}

void Trie::Print_File_Name(Trie_Node* tnode, vector<string>& History) //for search
{
	if (!tnode->link)
		return;

	//inorder to save the search histpry
	History.push_back(tnode->link->file_name);

	//printing the desired node's link list recursively
	cout << tnode->link->file_name << endl;
	Print_File_Name(tnode->link,History);
}

void Trie::Print_File_Name(Trie_Node* tnode) // for getting the list of files
{
	if (!tnode->link)
		return;
	cout << tnode->link->file_name << "  ";

	Print_File_Name(tnode->link);
}
////////////////////////////////////////// Functions used in the Main Menu //////////////////////////////////////////////

void Trie::Build(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
{

	vector<string> words;
	bool found;
	char B{};

	cout << "Press b to build the Trie" << endl;
	cin >> B;

	if (B == 'b')
	{
		// all files of the directory will be stored in files
		files = open(file_direction,found);

		if (found)
		{
			for (int i{ 2 }; i < files.size(); i++)
			{
				AddNewFile(file_direction, files[i], words, StopWords, Trie_File_counter, found);

				for (string& word : words)
					this->Add_Word(word, files[i]);

				for (int i{}; i < words.size(); i++)
					words.pop_back();
			}

			cout << " BST was Builed successfully , All existing files were added " << endl;
			cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
			cout << "number of files :" << Trie_File_counter << endl;
		}

	}
}

void Trie::Add_New_File(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
{
	string file_name{};
	vector<string> words;
	bool file_exists = false;
	bool found = true;

	system("cls");
	cout << "enter the file name" << endl;
	cin >> file_name;

	// check if the file is added before or not
	for (string& tmp : files)
	{
		if (tmp == file_name)
			file_exists = true;
	}

	if (file_exists)
	{
		cout << " This file already exists , you may want to update " << endl;
		return;
	}

	AddNewFile(file_direction, file_name, words, StopWords, Trie_File_counter, found);

	if (found)
	{
		// insert all the words to the tree iteratively
		for (string& word : words)
			this->Add_Word(word, file_name);

		// epmtify the vector of wors for next uses
		for (int i{}; i < words.size(); i++)
			words.pop_back();

		files.push_back(file_name);
		cout << " file added successfully " << endl;
		cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
		cout << "number of files :" << Trie_File_counter << endl;
	}
}

void Trie::Search(vector<string>& History)
{
	string search{};

	system("cls");
	cout << " Please enter your search word " << endl;
	cin >> search;

	find_file_name(search,History);

	//adding the new searchedd word to the history
	History.push_back(search);

	cout << " Recently Searches: " << endl;

	// printing the search history
	for (int i = History.size() - 1; i >= 0; i--)
	{
		cout << History[i] << endl;
	}
}

void Trie::Delete(vector<string>& files)
{
	string filename{};
	bool file_exists = false;


	system("cls");
	cout << " enter the file you want to delete : " << endl;
	cin >> filename;

	// check if the file exists or not
	for (string& tmp : files)
	{
		if (tmp == filename)
			file_exists = true;
	}

	if (!file_exists)
	{
		cout << " This File doesnt exists, you may want to add it!" << endl;
		return;
	}

	Delete_Word(root, filename);
	Trie_File_counter--;

	// remove that file from the vector of files
	for (int i{}; i < files.size() - 1; i++)
	{
		if (files[i] == filename)
		{
			files[i] = files[i + 1];
			filename = files[i + 1];
		}
	}

	files.pop_back();

	cout << "file removed successfully " << endl;
	cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
	cout << "number of files :" << Trie_File_counter << endl;
}

void Trie::Get_List_of_Words()
{
	system("cls");
	cout << " List of all existing words : " << endl;

	string prefix{};
	traverse(prefix, root);

	cout << " Numberof all existing words : " << Number_Of_Nodes(root) << " \n " << endl;
}

void Trie::Get_List_of_Files(vector<string>& files, const string& file_direction)
{
	bool found = true;

	system("cls");
	cout << " List of existing files : " << endl;

	//  print all the contents of files vector
	for (string& tmp : files)
		cout << tmp << endl;

	cout << " Number of existing files : " << files.size() - 2 << endl;
	cout << " List of all files : " << endl;

	// reading all the files in directory
	vector<string> All_Files = open(file_direction, found);

	//  print all the contents of all files vector
	for (string& tmp : All_Files)
		cout << tmp << endl;

	cout << " Number of existing files : " << All_Files.size() - 2 << " \n " << endl;
}

void Trie::Update(const string& file_direction, const vector<string>& StopWords)
{
	system("cls");
	string Name{};
	vector<string> words;
	bool found = true;

	cout << " Enter the name of file you want to update " << endl;
	cin >> Name;

	//deleting and reading the required file inorder to update it!
	Delete_Word(root, Name);
	Trie_File_counter--;
	AddNewFile(file_direction, Name, words, StopWords, Trie_File_counter, found);

	for (string& word : words)
		this->Add_Word(word, Name);

	for (int i{}; i < words.size(); i++)
		words.pop_back();

	cout << " File was updated successfully " << endl;
}


///////////////////////////////////////// Additional Helping Functions //////////////////////////////////////////

vector<string> Trie::open(std::string path, bool& found)
{
	DIR*    dir;
	dirent* pdir;
	std::vector<std::string> files;

	dir = opendir(path.c_str());

	if (!dir)
	{
		cout << " Unable to find this directory, Please try again " << endl;
		found = false;
		return files;
	}

	// reading the name of all files in a directory and push them to a vector
	while (pdir = readdir(dir))
		files.push_back(pdir->d_name);

	return files;
}

void Trie::traverse(std::string& prefix, Trie_Node* tnode)
{

	if (tnode->IsEndOfWord)
	{
		cout << "\n" << prefix << tnode->data << " : " << endl;
		Print_File_Name(tnode);
		cout << "\n";
	}
		
	// printing each node and it's link list
	for (int i{}; i < tnode->children.size(); i++)
	{
		char next = tnode->data;
		Trie_Node* pChild = tnode->children[i];

		if (pChild)
		{
			prefix.push_back(next);
			traverse(prefix, pChild);
			prefix.pop_back();
		}
	}

}

int Trie::Number_Of_Nodes(Trie_Node* tnode) //returns the number of words added
{
	int result = tnode->IsEndOfWord ? 1 : 0; // result is 1 if it is the end of word and else 0

	// travering all the childrn and gettin the number of their nodes recursively
	for (int i{}; i < tnode->children.size(); i++)
	{
		if (tnode->children[i] != NULL)
			result += Number_Of_Nodes(tnode->children[i]);
	}
		return result;
}

int Trie::height(Trie_Node* tnode)
{
	if (!tnode) 
		return 0;

	int max_height{};
	
	// the height of each subtree is the maximum height of it's children
	for (int i{}; i < tnode->children.size(); i++)
	{
		max_height = maximum(max_height, height(tnode->children[i]));
	}

	return 1 + max_height;
}

int maximum(int a, int b)
{
	return a ? a > b : b;
}

Trie_Node* Trie::getroot()
{
	return root;
}

Trie::~Trie()
{
}
