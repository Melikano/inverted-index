#include <string>
#include <iostream>
#include <time.h>
#include "BST.h"
#include "BST_Node.h"
#include "TST.h"
#include "Trie.h"
#include "Hash.h"
#include "StoringData.h"

using namespace std;


/////////////////////////////////// Main Menu in Main Function /////////////////////////////////////////

int main()
{
	vector<string> History; // to retain the history of search

	vector<string> StopWords;
	GetStopWords(StopWords);

	vector<string> files;
	string file_direction{};

	int&& choice{};

	cout << "\n--------------------------------" << endl;
	cout << "Inverted Index Implementation" << endl;
	cout << "\n--------------------------------" << endl;
	cout << " please choose the type of Data Structure : " << endl;
	cout << " 1.BST" << endl;
	cout << " 2.TST" << endl;
	cout << " 3.Trie" << endl;
	cout << " 4.Hash Table" << endl;

	cin >> choice;
	system("cls");

	cout << "enter file direction" << endl;
	cin.ignore();
	getline(cin, file_direction);

	system("cls");
			
	clock_t S_start, S_end, A_start, A_end, start, end;
	double Search_time{}, Add_time{}, time{};

	// BST :
	if (choice == 1)
	{
		start = clock();
		BST BS_Tree;

		A_start = clock();
		BS_Tree.Build(file_direction, StopWords, files);
		A_end = clock();

		int choicer{};
		int returner{};

		while (1)
		{
			cout << endl << endl;
			cout << " Please enter your request : " << endl;
			cout << " 1 . Add a new file " << endl;
			cout << " 2 . Delete a file " << endl;
			cout << " 3 . Search a word " << endl;
			cout << " 4 . Get List Of words " << endl;
			cout << " 5 . Get List Of Files " << endl;
			cout << " 6 . Update a file " << endl;
			cout << " 0 . Exit " << endl;

			cin >> choicer;

			switch (choicer)
			{
				case 1:
					BS_Tree.Add_New_File(file_direction, StopWords, files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 2:
					BS_Tree.Delete(files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 3:
					S_start = clock();
					BS_Tree.Search(History);
					S_end = clock();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");
					break;

				case 4:
					BS_Tree.Get_List_of_Words();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 5:
					BS_Tree.Get_List_of_Files(files,file_direction);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");
					break;

				case 6:
					BS_Tree.Update(file_direction,StopWords);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");
					break;

				case 0:
					break;

				default:
					break;
			}

			if (choicer == 0)
			{
				end = clock();
				Search_time = S_end - S_start;
				Add_time = A_end - A_start;
				time = end - start;
				cout << "total execution time = " << time << " ms " << endl;
				cout << "Search time of a word = " << Search_time << " ms " << endl;
				cout << "time of Adding files = " << Add_time << " ms " << endl;
				cout << "The height of the tree = " << BS_Tree.height(BS_Tree.getroot()) << endl;
				break;
			}
						
		}

	}

	// TST :
	if (choice == 2)
	{
		start = clock();
		TST TS_Tree;

		A_start = clock();
		TS_Tree.Build(file_direction, StopWords,files);
		A_end = clock();

		int choicer{};
		int returner{};

		while (1)
		{
			cout << endl << endl;
			cout << " Please enter your request : " << endl;
			cout << " 1 . Add a new file " << endl;
			cout << " 2 . Delete a file " << endl;
			cout << " 3 . Search a word " << endl;
			cout << " 4 . Get List Of words " << endl;
			cout << " 5 . Get List Of Files " << endl;
			cout << " 6 . Update a file " << endl;
			cout << " 0 . Exit " << endl;

			cin >> choicer;

			switch (choicer)
			{
				case 1:
					TS_Tree.Add_New_File(file_direction, StopWords,files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 2:
					TS_Tree.Delete(files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 3:
					S_start = clock();
					TS_Tree.Search(History);
					S_end = clock();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 4:
					TS_Tree.Get_List_of_Words();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 5:
					TS_Tree.Get_List_of_Files(files, file_direction);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");
					break;

				case 6:
					TS_Tree.Update(file_direction, StopWords);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 0:
					break;

				default:
					break;
			}

			if (choicer == 0)
			{
				end = clock();
				Search_time = S_end - S_start;
				Add_time = A_end - A_start;
				time = end - start;
				cout << "total execution time = " << time << " ms " << endl;
				cout << "Search time of a word = " << Search_time << " ms " << endl;
				cout << "time of Adding files = " << Add_time << " ms " << endl;
				cout << "The height of the tree = " << TS_Tree.height(TS_Tree.getroot()) << endl;
				break;
			}
		}

	}

	//Trie :
	if (choice == 3)
	{
		start = clock();
		Trie Trie_Tree;

		A_start = clock();
		Trie_Tree.Build(file_direction, StopWords,files);
		A_end = clock();

		int choicer{};
		int returner{};

		while (1)
		{
			cout << endl << endl;
			cout << " Please enter your request : " << endl;
			cout << " 1 . Add a new file " << endl;
			cout << " 2 . Delete a file " << endl;
			cout << " 3 . Search a word " << endl;
			cout << " 4 . Get List Of words " << endl;
			cout << " 5 . Get List Of Files " << endl;
			cout << " 6 . Update a file " << endl;
			cout << " 0 . Exit " << endl;

			cin >> choicer;

			switch (choicer)
			{
				case 1:
					Trie_Tree.Add_New_File(file_direction, StopWords,files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

				break;

				case 2:
					Trie_Tree.Delete(files);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 3:
					S_start = clock();
					Trie_Tree.Search(History);
					S_end = clock();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;
				case 4:
					Trie_Tree.Get_List_of_Words();

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 5:
					Trie_Tree.Get_List_of_Files(files,file_direction);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");

					break;

				case 6:
					Trie_Tree.Update(file_direction,StopWords);

					cout << "\n Press 0 to return " << endl;
					cin >> returner;
					if (returner == 0)
						system("cls");
						
					break;

				case 0:
					break;

				default:
					break;
			}

			if (choicer == 0)
			{
				end = clock();
				Search_time = S_end - S_start;
				Add_time = A_end - A_start;
				time = end - start;
				cout << "total execution time = " << time << " ms " << endl;
				cout << "Search time of a word = " << Search_time << " ms " << endl;
				cout << "time of Adding files = " << Add_time << " ms " << endl;
				cout << "The height of the tree = " << Trie_Tree.height(Trie_Tree.getroot()) << endl;
				break;
			}
		}
	}

	if (choice == 4)
	{
		start = clock();
		Hash Hash_Table;

		A_start = clock();
		Hash_Table.Build(file_direction, StopWords, files);
		A_end = clock();

		int choicer{};
		int returner{};

		while (1)
		{
			cout << endl << endl;
			cout << " Please enter your request : " << endl;
			cout << " 1 . Add a new file " << endl;
			cout << " 2 . Delete a file " << endl;
			cout << " 3 . Search a word " << endl;
			cout << " 4 . Get List Of words " << endl;
			cout << " 5 . Get List Of Files " << endl;
			cout << " 6 . Update a file " << endl;
			cout << " 0 . Exit " << endl;

			cin >> choicer;

			switch (choicer)
			{
			case 1:
				Hash_Table.Add_New_File(file_direction, StopWords, files);

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");

				break;

			case 2:
				Hash_Table.Delete(files);

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");

				break;

			case 3:
				S_start = clock();
				Hash_Table.Search(History);
				S_end = clock();

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");
				break;

			case 4:
				Hash_Table.Get_List_of_Words();

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");

				break;

			case 5:
				Hash_Table.Get_List_of_Files(files, file_direction);

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");
				break;

			case 6:
				Hash_Table.Update(file_direction, StopWords);

				cout << "\n Press 0 to return " << endl;
				cin >> returner;
				if (returner == 0)
					system("cls");
				break;

			case 0:
				break;

			default:
				break;
			}

			if (choicer == 0)
			{
				end = clock();
				Search_time = S_end - S_start;
				Add_time = A_end - A_start;
				time = end - start;
				cout << "total execution time = " << time << " ms " << endl;
				cout << "Search time of a word = " << Search_time << " ms " << endl;
				cout << "time of Adding files = " << Add_time << " ms " << endl;
				break;
			}

		}

	}

	return 0;
}

