#pragma once

#include<string>

class BST_Node
{

private:
	std::string data;
	BST_Node* Lc;
	BST_Node* Rc;
	BST_Node* Mc;
	BST_Node* link{ nullptr };

public:
	BST_Node();
	~BST_Node();
	friend class BST;
	friend class TST;

};

