#include "Hash_Node.h"


Hash_Node::Hash_Node()
{
}

Hash_Node::Hash_Node(int key, string data)
{
	this->data = data;
	this->key = key;
	this->link = NULL;
}


Hash_Node::~Hash_Node()
{
}
