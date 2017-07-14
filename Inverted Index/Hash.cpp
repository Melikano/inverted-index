#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include "Hash.h"
#include "Hash_Node.h"
#include "StoringData.h"

int TABLE_SIZE{ 200 };
int Hash_File_counter{};

Hash::Hash()
{
	htable = new Hash_Node*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		htable[i] = NULL;
	}
}

//////////////////////////////////////////// Functions For Adding & Deleteing Files //////////////////////////////////

void Hash::Add_File_Name(Hash_Node* hnode, string FileName)
{
	// make a new node in files ink list
	Hash_Node* NewFileName = new Hash_Node;
	NewFileName->data = FileName;

	// find the right place for the new node
	while (hnode->link)
	{
		hnode = hnode->link;

		if (hnode->data == FileName)
			return;
	}

	hnode->link = NewFileName;
}

void Hash::Delete_File_Name(Hash_Node* hnode, string FileName)
{
	Hash_Node* CurrentNode{ hnode->link };
	Hash_Node* PreviousNode{ NULL };

	// finding the desired node and delete it
	while (CurrentNode)
	{
		if (CurrentNode->data == FileName)
		{
			if (PreviousNode)
				PreviousNode->link = CurrentNode->link;
			else
				hnode->link = CurrentNode->link;
			delete CurrentNode;
			return;
		}

		PreviousNode = CurrentNode;
		CurrentNode = CurrentNode->link;
	}
}

int Hash::Number_Of_Files(Hash_Node* hnode)
{
	if (!hnode)
		return 0;
	return 1 + Number_Of_Files(hnode->link);
}

//////////////////////////////////////// Functions For Adding & Deleting Word /////////////////////////////////////////////

void Hash::Add_Word(string NewWord, string FileName)
{
	int key{};
	bool NoSpace{ false };
	for (char tmp : NewWord)
		key += static_cast<int>(tmp);

	int index{ HashFunc(key) };

	do
	{
		if (htable[index] == NULL)
		{
			htable[index] = new Hash_Node(key, NewWord); // if it was empty put the word in it 
			Add_File_Name(htable[index], FileName);
			return;
		}

		if (htable[index]->data == NewWord) // if it was added before return
		{
			Add_File_Name(htable[index], FileName);
			return;
		}

		index = (index + 1) % TABLE_SIZE;
	} while (index != HashFunc(key));

	cout << "not enough space " << endl;
}

void Hash::Delete_Word(string FileName)
{
	for (int i{}; i < TABLE_SIZE; i++)
	{
		if (htable[i] != NULL)
		{
			Delete_File_Name(htable[i], FileName); // first we remove the file name from each word's linklist

			if (Number_Of_Files(htable[i]->link) == 0)
			{
				htable[i] = NULL;

				if (htable[i + 1])
				{
					int index{ HashFunc(htable[i + 1]->key) };

					do
					{
						if (htable[index] == NULL)
						{
							htable[index] = htable[index + 1];
							htable[index + 1] = NULL;
							index = (index + 1) % TABLE_SIZE;
						}
						else
							break;

					} while (index != HashFunc(htable[i + 1]->key));// rearrange the hash table
				}
			}
		}
	}
}

int Hash::HashFunc(int key)
{
	return key % TABLE_SIZE;
}

/////////////////////////////////////////// Functions For Search //////////////////////////////////////////////

void Hash::find_file_name(string SearchedWord, vector<string>& History)
{
	int key{};

	for (char tmp : SearchedWord)
		key += static_cast<int>(tmp);

	int index{ HashFunc(key) };

	do
	{
		if (htable[index])
		{
			if (htable[index]->data == SearchedWord)
			{
				cout << "found" << endl;
				Print_File_Name(htable[index], History);
				return;
			}
		}

		index = (index + 1 ) % TABLE_SIZE;

	} while (index != HashFunc(key));

	cout << "not found" << endl;
	History.push_back("not found");
}

void Hash::Print_File_Name(Hash_Node* hnode, vector<string>& History) //for search
{
	if (!hnode->link)
		return;

	//inorder to save the search histpry
	History.push_back(hnode->link->data);

	//printing the desired node's link list recursively
	cout << hnode->link->data << endl;
	Print_File_Name(hnode->link, History);

}
void Hash::Print_File_Name(Hash_Node* bnode) // for getting the list of files
{
	if (!bnode->link)
		return;
	cout << bnode->link->data << "  ";

	Print_File_Name(bnode->link);
}

///////////////////////////////////////// Functions used in the Main Menu ////////////////////////////////////////////

void Hash::Build(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
{
	vector<string> words;
	bool found = true;
	char B{};

	cout << "Press b to build the Hash" << endl;
	cin >> B;

	if (B == 'b')
	{
		// all files of the directory will be stored in files
		files = open(file_direction, found);

		if (found)
		{
			for (int i{ 2 }; i < files.size(); i++)
			{
				AddNewFile(file_direction, files[i], words, StopWords, Hash_File_counter, found);

				for (string& word : words)
					this->Add_Word(word, files[i]);

				for (int i{}; i < words.size(); i++)
					words.pop_back();
			}

			cout << " Hash was Builed successfully , All existing files were added " << endl;
			cout << "number of words : " << this->Number_Of_Words() << endl;
			cout << "number of files :" << Hash_File_counter << endl;

		}
	}
}

void Hash::Add_New_File(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
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

	AddNewFile(file_direction, file_name, words, StopWords, Hash_File_counter, found);

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
		cout << "number of words : " << this->Number_Of_Words() << endl;
		cout << "number of files :" << Hash_File_counter << endl;
	}

}

void Hash::Search(vector<string>& History)
{
	string search{};


	system("cls");
	cout << " Please enter your search word " << endl;
	cin >> search;

	find_file_name(search, History);

	//adding the new searchedd word to the history
	History.push_back(search);

	cout << " Recent Searches: " << endl;

	// printing the search history
	for (int i = History.size() - 1; i >= 0; i--)
		cout << History[i] << endl;
}

void Hash::Delete(vector<string>& files)
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

	Delete_Word(filename);
	Hash_File_counter--;

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

	cout << "file was removed successfully " << endl;
	cout << "number of words : " << this->Number_Of_Words()<< endl;
	cout << "number of files :" << Hash_File_counter << endl;
}

void Hash::Get_List_of_Words()
{
	system("cls");
	cout << " List of all existing words : " << endl;
	traverse();
	cout << " Numberof all existing words : " << Number_Of_Words() << " \n " << endl;
}

void Hash::Get_List_of_Files(vector<string>& files, const string& file_direction)
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
void Hash::Update(const string& file_direction, const vector<string>& StopWords)
{
	system("cls");

	string Name{};
	vector<string> words;
	bool found = true;

	cout << " Enter the name of file you want to update " << endl;
	cin >> Name;

	// deleting and reading the required file inorder to update it!
	Delete_Word(Name);
	Hash_File_counter--;

	AddNewFile(file_direction, Name, words, StopWords, Hash_File_counter, found);

	for (string& word : words)
		this->Add_Word(word, Name);

	for (int i{}; i < words.size(); i++)
		words.pop_back();

	cout << " File was updated successfully " << endl;
}

///////////////////////////////////////// Additional Helping Functions //////////////////////////////////////////

vector<string> Hash::open(string path, bool& found)
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

int Hash::Number_Of_Words()
{
	int counter{};
	for (int i{}; i < TABLE_SIZE; i++)
	{
		if (htable[i] != NULL)
			counter++;
	}

	return counter;
}

void Hash::traverse()
{
	for (int i{}; i < TABLE_SIZE; i++)
	{
		if (htable[i] != NULL)
		{
			cout << htable[i]->data << " : " << endl;
			Print_File_Name(htable[i]);
			cout << endl;
		}
	}
}
Hash::~Hash()
{
}
