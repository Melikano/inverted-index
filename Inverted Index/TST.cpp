#include <string>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include "TST.h"
#include "BST_Node.h"
#include "StoringData.h"

int TsT_File_counter{};

using namespace std;

TST::TST()
{
}


////////////////////////////////////////////// Functions For Adding & Deleteing Files //////////////////////////////////

void TST::Add_File_Name(BST_Node* bnode, string FileName)
{
	// make a new node in files ink list
	BST_Node* NewFileName = new BST_Node;
	NewFileName->data = FileName;

	// find the right place for the new node
	while (bnode->link)
	{
		bnode = bnode->link;

		if (bnode->data == FileName)
			return;
	}

	bnode->link = NewFileName;
}

void TST::Delete_File_Name(BST_Node* bnode, string FileName)
{
	BST_Node* CurrentNode{ bnode->link };
	BST_Node* PreviousNode{ nullptr };

	// finding the desired node and delete it
	while (CurrentNode)
	{
		if (CurrentNode->data == FileName)
		{
			if (PreviousNode)
				PreviousNode->link = CurrentNode->link;
			else
				bnode->link = CurrentNode->link;
			delete CurrentNode;
			return;
		}
		PreviousNode = CurrentNode;
		CurrentNode = CurrentNode->link;
	}
}

int TST::Number_Of_Files(BST_Node* bnode)
{
	if (!bnode)
		return 0;

	return 1 + Number_Of_Files(bnode->link);
}


//////////////////////////////////////// Functions For Adding & Deleting Word /////////////////////////////////////////////

BST_Node* TST::Add_Word(BST_Node *bnode, string NewWord, string FileName)
{
	static bool Isroot = true;
	static int index{};

	// creating a new node after finding the right place
	if (!bnode)
	{
		bnode = new BST_Node;
		bnode->data = NewWord;
		bnode->Lc = NULL;
		bnode->Rc = NULL;
		bnode->Mc = NULL;
		bnode->link = NULL;

		Add_File_Name(bnode, FileName);

		// the first node added will be the root
		if (Isroot)
		{
			root = bnode;
			Isroot = false;
		}
		return bnode;
	}
	// if data is smaller 
	if (NewWord < bnode->data)
	{
		// check if it is the equal child or not
		if (bnode->data[index] == NewWord[index])
		{
			index++;
			bnode->Mc = Add_Word(bnode->Mc, NewWord, FileName);
		}
		else
		{
			bnode->Lc = Add_Word(bnode->Lc, NewWord, FileName);
			bnode = Make_Balance(bnode);
		}
	}

	// if data is bigger 
	else if (NewWord > bnode->data)
	{
		// check if it is the equal child or not
		if (bnode->data[index] == NewWord[index])
		{
			index++;
			bnode->Mc = Add_Word(bnode->Mc, NewWord, FileName);
		}
		else
		{
			bnode->Rc = Add_Word(bnode->Rc, NewWord, FileName);
			bnode = Make_Balance(bnode);
		}
	}

	// if this file is already added, just add the file name
	else if (NewWord == bnode->data)
		Add_File_Name(bnode, FileName);

	index = 0;
	return bnode;
}

void TST::Add_Word(string NewWord, string FileName)
{
	Add_Word(root, NewWord, FileName);
	return;
}

void TST::Delete_Word(BST_Node* tnode, string FileName)
{
	if (!tnode)
		return;

	static bool IsDeleted{ false };

	Delete_Word(tnode->Lc, FileName);

	// if the left child is deleted so its pointer should point to NULL
	if (IsDeleted)
	{
		tnode->Lc = NULL;
		IsDeleted = false;
	}

	Delete_Word(tnode->Mc, FileName);

	// if the equal child is deleted so its pointer should point to NULL
	if (IsDeleted)
	{
		tnode->Mc = NULL;
		IsDeleted = false;
	}

	Delete_Word(tnode->Rc, FileName);

	if (IsDeleted)
		// if the right child is deleted so its pointer should point to NULL
	{
		tnode->Rc = NULL;
		IsDeleted = false;
	}

	// deleting the desired file from the link list of all nodes recursively
	Delete_File_Name(tnode, FileName);

	// if a node only belongs to that file, it should be deleted
	if (Number_Of_Files(tnode->link) == 0)
	{
		BST_Node* tmp = tnode;

		while (tmp->Lc || tmp->Mc || tmp->Rc)
		{
			// if it has equal child we should replace it's data withe the equal child
			if (tmp->Mc)
			{
				tmp->data = tmp->Mc->data;
				tmp = tmp->Mc;
			}

			// if it has right subtree we should replace it's data with the min_right
			if (tnode->Rc)
			{
				BST_Node* Min_Right = Min_Node(tnode->Rc);
				tmp->data = Min_Right->data;
				tmp = Min_Right;
			}

			// if it has left subtree we should replace it's data with the max_left
			else if (tnode->Lc)
			{
				BST_Node* Max_Left = Max_Node(tnode->Lc);
				tmp->data = Max_Left->data;
				tmp = Max_Left;
			}
		}

		IsDeleted = true;
		tmp = NULL;
		delete tmp;

	}

	return;

}

void TST::Delete_Word(string FileName)
{
	Delete_Word(root, FileName);
}

//////////////////////////////////////////////  Functions For Balancing The BST  //////////////////////////////////////////////


int TST::Balance_Factor(BST_Node* tnode)
{
	return height(tnode->Lc) - height(tnode->Rc);
}


BST_Node* TST::RR_Rotation(BST_Node* bnode)  //Right-Right Rotation 
{
	BST_Node* temp;
	temp = bnode->Rc;
	bnode->Rc = temp->Lc;
	temp->Lc = bnode;
	if (root == bnode)
		root = temp;
	return temp;
}

BST_Node* TST::LL_Rotation(BST_Node* bnode) //Left-Left Rotation
{
	BST_Node* temp;
	temp = bnode->Lc;
	bnode->Lc = temp->Rc;
	temp->Rc = bnode;
	if (root == bnode)
		root = temp;
	return temp;
}

BST_Node* TST::LR_Rotation(BST_Node* bnode) //Left-Right Rotation
{
	BST_Node* temp;
	temp = bnode->Lc;
	bnode->Lc = RR_Rotation(temp);
	return LL_Rotation(bnode);
}

BST_Node* TST::RL_Rotation(BST_Node* bnode) //Right-Left Rotation
{
	BST_Node* temp;
	temp = bnode->Rc;
	bnode->Rc = LL_Rotation(temp);
	return RR_Rotation(bnode);
}

BST_Node* TST::Make_Balance(BST_Node *bnode) //making Rotations under differnet situations
{
	int b_factor = Balance_Factor(bnode);

	if (b_factor > 1)
	{
		if (Balance_Factor(bnode->Lc) >= 0)
			bnode = LL_Rotation(bnode);
		else
			bnode = LR_Rotation(bnode);
	}

	else if (b_factor < -1)
	{
		if (Balance_Factor(bnode->Rc) > 0)
			bnode = RL_Rotation(bnode);
		else
			bnode = RR_Rotation(bnode);
	}

	return bnode;
}


/////////////////////////////////////////// Functions For Search //////////////////////////////////////////////


BST_Node* TST::find_file_name(string SearchedWord, BST_Node* bnode, vector<string>& History)
{
	static int index{};

	if (!bnode)  // not found
	{
		cout << "notfound" << endl;
		History.push_back("not found");
		return NULL;
	}

	if (bnode->data == SearchedWord) // found
	{
		cout << "found" << endl;
		Print_File_Name(bnode, History);
		index = 0;
		return bnode;
	}

	if (bnode->data > SearchedWord)
	{
		if (bnode->data[index] == SearchedWord[index])
		{
			index++;
			return find_file_name(SearchedWord, bnode->Mc, History);
			
		}
		else
			return find_file_name(SearchedWord, bnode->Lc, History);
	}

	else
	{
		if (bnode->data[index] == SearchedWord[index])
		{
			index++;
			return find_file_name(SearchedWord, bnode->Mc, History);
		}

		else
			return find_file_name(SearchedWord, bnode->Rc, History);
	}
}

void TST::FindFileName(string SearchedWord, vector<string>& History)
{
	find_file_name(SearchedWord, root, History);
}

void TST::Print_File_Name(BST_Node* bnode, vector<string>& History) // for search
{
	if (!bnode->link)
		return;

	//inorder to save the search histpry
	History.push_back(bnode->link->data);

	//printing the desired node's link list recursively
	cout << bnode->link->data << endl;
	Print_File_Name(bnode->link,History);
}

void TST::Print_File_Name(BST_Node* bnode) // for getting the list of files
{
	if (!bnode->link)
		return;

	cout << bnode->link->data << "  ";
	Print_File_Name(bnode->link);
}


////////////////////////////////////////// Functions used in the Main Menu /////////////////////////////////////////////////////

void TST::Build(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
{

	vector<string> words;
	char B{};
	bool found = true;

	cout << "Press b to build the TST" << endl;
	cin >> B;

	if (B == 'b')
	{
		// all files of the directory will be stored in files
		files = open(file_direction, found);

		if (found)
		{
			for (int i{ 2 }; i < files.size(); i++)
			{
				AddNewFile(file_direction, files[i], words, StopWords, TsT_File_counter, found);

				for (string& word : words)
					this->Add_Word(word, files[i]);

				for (int i{}; i < words.size(); i++)
					words.pop_back();
			}

			cout << " BST was Builed successfully , All existing files were added " << endl;
			cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
			cout << "number of files :" << TsT_File_counter << endl;
		}
	}
}

void TST::Add_New_File(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
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

	AddNewFile(file_direction, file_name, words, StopWords, TsT_File_counter, found);

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
		cout << "number of files :" << TsT_File_counter << endl;
	}

}

void TST::Search(vector<string>& History)
{
	string search{};

	system("cls");
	cout << " Please enter your search word " << endl;
	cin >> search;

	FindFileName(search,History);

	//adding the new searchedd word to the history
	History.push_back(search);

	cout << " Recently Searches: " << endl;

	// printing the search history
	for (int i = History.size() - 1; i >= 0; i--)
	{
		cout << History[i] << endl;
	}
}

void TST::Delete(vector<string>& files)
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
	TsT_File_counter--;

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

	cout << "file deleted successfully " << endl;
	cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
	cout << "number of files :" << TsT_File_counter << endl;
}

void TST::Get_List_of_Words()
{
	system("cls");
	cout << " List of all existing words : " << endl;
	inorder(root);
	cout << " Numberof all existing words : " << Number_Of_Nodes(root) << " \n " << endl;
}

void TST::Get_List_of_Files(vector<string>& files, const string& file_direction)
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
	vector<string> All_Files = open(file_direction,found);

	//  print all the contents of all files vector
	for (string& tmp : All_Files)
		cout << tmp << endl;

	cout << " Number of existing files : " << All_Files.size() - 2 << " \n " << endl;
}


void TST::Update(const string& file_direction, const vector<string>& StopWords)
{
	system("cls");
	string Name{};
	vector<string> words;
	bool found = true;

	cout << " Enter the name of file you want to update " << endl;
	cin >> Name;
	// deleting and reading the required file inorder to update it!
	Delete_Word(root, Name);
	TsT_File_counter--;
	AddNewFile(file_direction, Name, words, StopWords, TsT_File_counter,found);

	for (string& word : words)
		this->Add_Word(word, Name);

	for (int i{}; i < words.size(); i++)
		words.pop_back();

	cout << " File was updated successfully " << endl;
}

///////////////////////////////////////// Additional Helping Functions //////////////////////////////////////////

vector<string> TST::open(string path, bool& found)
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

void TST::inorder(BST_Node *tree)
{
	if (!tree)
		return;
	inorder(tree->Lc);
	inorder(tree->Mc);

	// inorder travesal and printing each node's link list
	cout << tree->data << " : " << endl;
	Print_File_Name(tree);

	cout << endl << endl;

	inorder(tree->Rc);
}

int TST::height(BST_Node* bnode)
{
	if (!bnode)
		return 0;

	int left_h = height(bnode->Lc);
	int right_h = height(bnode->Rc);
	int mid_h = height(bnode->Mc);

	// the height of each subtree is the maximum height of children
	if (left_h > right_h) 
	{
		if (left_h > mid_h)
			return left_h + 1;
		else
			return mid_h + 1;
	}
	else
	{
		if (right_h > mid_h)
			return right_h + 1;
		else
			return mid_h + 1;
	}
		
}

int TST::Number_Of_Nodes(BST_Node* bnode)   //returns the number of words added
{
	if (!bnode)
		return 0;
	return 1 + Number_Of_Nodes(bnode->Rc) + Number_Of_Nodes(bnode->Lc) + Number_Of_Nodes(bnode->Mc);
}

BST_Node* TST::Min_Node(BST_Node* bnode)
{
	BST_Node* CurrentNode = bnode;

	while (CurrentNode->Lc)
		CurrentNode = CurrentNode->Lc;

	return CurrentNode;
}

BST_Node* TST::Max_Node(BST_Node* bnode)
{
	BST_Node* CurrentNode = bnode;
	while (CurrentNode->Rc)
		CurrentNode = CurrentNode->Rc;

	return CurrentNode;
}

BST_Node* TST::getroot()
{
	return root;
}

TST::~TST()
{
}
