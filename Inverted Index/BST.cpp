#include <string>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include "BST.h"
#include "BST_Node.h"
#include "StoringData.h"

using namespace std;

int File_counter{};


BST::BST()
{
}


 //////////////////////////////////////////// Functions For Adding & Deleteing Files //////////////////////////////////

void BST::Add_File_Name(BST_Node* bnode, string FileName)
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

void BST::Delete_File_Name(BST_Node* bnode,string FileName)
{
	BST_Node* CurrentNode{ bnode->link };
	BST_Node* PreviousNode{ NULL };

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

int BST::Number_Of_Files(BST_Node* bnode)
{
	if (!bnode)
		return 0;
	return 1 + Number_Of_Files(bnode->link);
}

//////////////////////////////////////// Functions For Adding & Deleting Word /////////////////////////////////////////////

BST_Node* BST::Add_Word(BST_Node *bnode, string NewWord, string FileName)
{
	static bool Isroot = true;

	// creating a new node after finding the right place
	if ( !bnode )
	{
		bnode = new BST_Node;
		bnode->data = NewWord;
		bnode->Lc = NULL;
		bnode->Rc = NULL;
		bnode->link = NULL;

		Add_File_Name(bnode,FileName);

		// the first node added will be the root
		if (Isroot)
		{
			root = bnode;
			Isroot = false;
		}

		return bnode;
	}

	// if data is smaller than each root go to left subtree
	else if (NewWord < bnode->data)
	{
		bnode->Lc = Add_Word(bnode->Lc, NewWord,FileName);
		bnode = Make_Balance(bnode);
	}

	// if data is bigger than each root go to lright subtree
	else if (NewWord > bnode->data)
	{
		bnode->Rc = Add_Word(bnode->Rc, NewWord, FileName);
		bnode = Make_Balance(bnode);
	}

	// if this file is already added, just add the file name
	else if (NewWord == bnode->data)
	{
		Add_File_Name(bnode, FileName);
	}

	return bnode;
}

void BST::Add_Word(string NewWord, string FileName)
{
	Add_Word(root, NewWord, FileName);
	return;
}

void BST::Delete_Word(BST_Node* bnode, string FileName)
{
	if (!bnode)
		return;

	static bool IsDeleted{ false };

	Delete_Word(bnode->Lc, FileName);

	// if the left child is deleted so its pointer should point to NULL
	if (IsDeleted)
	{
		bnode->Lc = NULL;
		IsDeleted = false;
	}

	Delete_Word(bnode->Rc, FileName);

	// if the right child is deleted so its pointer should point to NULL
	if (IsDeleted)
	{
		bnode->Rc = NULL;
		IsDeleted = false;
	}

	// deleting the desired file from the link list of all nodes recursively
	Delete_File_Name(bnode, FileName);

	// if a node only belongs to that file, it should be deleted
	if ( Number_Of_Files(bnode->link) == 0)
	{
		BST_Node* tmp = bnode;
		while (tmp->Lc || tmp->Rc)
		{
			// if it has right subtree we should replace it's data with the min_right
			if (tmp->Rc)
			{
				BST_Node* Min_Right = Min_Node(bnode->Rc);
				tmp->data = Min_Right->data;
				tmp = Min_Right;
			}

			// if it has left subtree we should replace it's data with the max_left
			else if (tmp->Lc)
			{
				BST_Node* Max_Left = Max_Node(bnode->Lc);
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


void BST::Delete_Word(string FileName)
{
	Delete_Word(root, FileName);
}

//////////////////////////////////////////////  Functions For Balancing The BST  //////////////////////////////////////////////


int BST::Balance_Factor(BST_Node* bnode)
{
	return height(bnode->Lc) - height(bnode->Rc);
}

BST_Node* BST::RR_Rotation(BST_Node* bnode)  //Right-Right Rotation 
{
	BST_Node* temp;
	temp = bnode->Rc;
	bnode->Rc = temp->Lc;
	temp->Lc = bnode;
	if (root == bnode)
		root = temp;
	return temp;
}

BST_Node* BST::LL_Rotation(BST_Node* bnode) //Left-Left Rotation
{
	BST_Node* temp;
	temp = bnode->Lc;
	bnode->Lc = temp->Rc;
	temp->Rc = bnode;
	if (root == bnode)
		root = temp;
	return temp;
}

BST_Node* BST::LR_Rotation(BST_Node* bnode) //Left-Right Rotation
{
	BST_Node* temp;
	temp = bnode->Lc;
	bnode->Lc = RR_Rotation(temp);
	return LL_Rotation(bnode);
}

BST_Node* BST::RL_Rotation(BST_Node* bnode) //Right-Left Rotation
{
	BST_Node* temp;
	temp = bnode->Rc;
	bnode->Rc = LL_Rotation(temp);
	return RR_Rotation(bnode);
}

BST_Node* BST::Make_Balance(BST_Node *bnode) //making Rotations under differnet situations
{
	int b_factor = Balance_Factor(bnode);
	if (b_factor > 1)
	{
		if (Balance_Factor(bnode->Lc) > 0)
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


BST_Node* BST::find_file_name(string SearchedWord, BST_Node* bnode, vector<string>& History)
{
	
	if (!bnode)  // not found
	{
		cout << "notfound" << endl;
		History.push_back("not found");
		return NULL;
	}

	if (bnode->data == SearchedWord) // found
	{
		cout << "found" << endl;
		Print_File_Name(bnode,History);
		return bnode;
	}

	//Searching the Left subtree
	if (bnode->data > SearchedWord)  
		return find_file_name(SearchedWord, bnode->Lc, History);

	//Searching the right subtree 
	else       
		return find_file_name(SearchedWord, bnode->Rc, History);
}

void BST:: FindFileName(string SearchedWord, vector<string>& History)
{
	find_file_name(SearchedWord, root, History);
}

void BST::Print_File_Name(BST_Node* bnode, vector<string>& History) //for search
{
	if (!bnode->link)
		return;

	//inorder to save the search histpry
	History.push_back(bnode->link->data);

	//printing the desired node's link list recursively
	cout << bnode->link->data << endl;
	Print_File_Name(bnode->link,History);
}

void BST::Print_File_Name(BST_Node* bnode) // for getting the list of files
{
	if (!bnode->link)
		return;
	cout << bnode->link->data << "  " ;

	Print_File_Name(bnode->link);
}


///////////////////////////////////////// Functions used in the Main Menu ////////////////////////////////////////////

void BST::Build(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
{
	vector<string> words;
	bool found = true;
	char B{};

	cout << "Press b to build the BST" << endl;
	cin >> B;

	if (B == 'b')
	{
		// all files of the directory will be stored in files
		files = open(file_direction, found);

		if (found)
		{
			for (int i{ 2 }; i < files.size(); i++)
			{
				AddNewFile(file_direction, files[i], words, StopWords, File_counter,found);

				for (string& word : words)
					this->Add_Word(word, files[i]);

				for (int i{}; i < words.size(); i++)
					words.pop_back();
			}

			cout << " BST was Builed successfully , All existing files were added " << endl;
			cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
			cout << "number of files :" << File_counter << endl;

		}
	}
}

void BST::Add_New_File(const string& file_direction, const vector<string>& StopWords, vector<string>& files)
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

	AddNewFile(file_direction, file_name, words, StopWords, File_counter,found);

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
		cout << "number of files :" << File_counter << endl;
	}


}

void BST::Search(vector<string>& History)
{
	string search{};


	system("cls");
	cout << " Please enter your search word " << endl;
	cin >> search;

	FindFileName(search,History);

	//adding the new searchedd word to the history
	History.push_back(search);

	cout << " Recent Searches: " << endl;

	// printing the search history
	for (int i = History.size() - 1  ; i >= 0 ; i--)
		cout << History[i] << endl;
}

void BST::Delete(vector<string>& files)
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
	File_counter--;

	// remove that file from the vector of files
	for (int i{}; i < files.size() - 1 ; i++)
	{
		if (files[i] == filename)
		{
			files[i] = files[i + 1];
			filename = files[i + 1];
		}
	}
	
	files.pop_back();

	cout << "file was removed successfully " << endl;
	cout << "number of words : " << this->Number_Of_Nodes(root) << endl;
	cout << "number of files :" << File_counter << endl;
}


void BST::Get_List_of_Words()
{
	system("cls");
	cout << " List of all existing words : " << endl;
	inorder(root);
	cout << " Numberof all existing words : " << Number_Of_Nodes(root) << " \n " << endl;
}

void BST::Get_List_of_Files(vector<string>& files, const string& file_direction)
{
	bool found = true;

	system("cls");
	cout << " List of existing files : " << endl;

	//  print all the contents of files vector
	for (string& tmp : files)
		cout << tmp << endl;

	cout << " Number of existing files : " << files.size() - 2<<  endl;

	cout << " List of all files : " << endl;
	
	// reading all the files in directory
	vector<string> All_Files = open(file_direction,found);

	//  print all the contents of all files vector
	for (string& tmp : All_Files)
		cout << tmp << endl;

	cout << " Number of existing files : " << All_Files.size() - 2 << " \n " << endl;
}

void BST::Update(const string& file_direction, const vector<string>& StopWords)
{
	system("cls");

	string Name{};
	vector<string> words;
	bool found = true;

	cout << " Enter the name of file you want to update " << endl;
	cin >> Name;

	 // deleting and reading the required file inorder to update it!
	Delete_Word(root, Name);
	File_counter--;

	AddNewFile(file_direction, Name, words, StopWords, File_counter, found);

	for (string& word : words)
		this->Add_Word(word, Name);

	for (int i{}; i < words.size(); i++)
		words.pop_back();

	cout << " File was updated successfully " << endl;
}
///////////////////////////////////////// Additional Helping Functions //////////////////////////////////////////

vector<string> BST::open(string path,bool& found) 
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

void BST::inorder(BST_Node *tree)
{
	if (!tree)
		return;
	inorder(tree->Lc);
	 
	// inorder travesal and printing each node's link list
	cout << tree->data << " : " << endl;
	Print_File_Name(tree);

	cout << endl << endl;

	inorder(tree->Rc);
}

int BST::height(BST_Node* bnode)
{
	if (!bnode)
		return 0;

	int left_h = height(bnode->Lc);
	int right_h = height(bnode->Rc);

	// the height of each subtree is the maximum height of children
	if (left_h > right_h)
		return left_h + 1;

	else
		return right_h + 1;
}

int BST::Number_Of_Nodes(BST_Node* bnode)  //returns the number of words added
{
	if (!bnode)
		return 0;
	return 1 + Number_Of_Nodes(bnode->Rc) + Number_Of_Nodes(bnode->Lc);
}

BST_Node* BST::Min_Node(BST_Node* bnode) // finds the minimum node
{
	BST_Node* CurrentNode = bnode;

	while (CurrentNode->Lc)
		CurrentNode = CurrentNode->Lc;
	return CurrentNode;
}

BST_Node* BST::Max_Node(BST_Node* bnode) // fins the maximum node
{
	BST_Node* CurrentNode = bnode;
	while (CurrentNode->Rc)
		CurrentNode = CurrentNode->Rc;
	return CurrentNode;
}

BST_Node* BST::getroot()
{
	return root;
}

BST::~BST()
{

}
