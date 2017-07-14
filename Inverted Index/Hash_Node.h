#pragma once
#include <string>
#include <vector>

using namespace std;

class Hash_Node
{

private:
	int key;
	string data;
	Hash_Node* link;
public:
	Hash_Node();
	Hash_Node(int, string);
	friend class Hash;
	~Hash_Node();

};

