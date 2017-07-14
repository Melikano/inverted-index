#include "Trie_Node.h"



Trie_Node::Trie_Node()
{
	IsEndOfWord = false;
	link = NULL;
}

Trie_Node* Trie_Node::findChild(char c)
{
	for (int i = 0; i < children.size(); i++)
	{
		Trie_Node* tmp = children.at(i);

		if(tmp)
			if (tmp->data == c)
				return tmp;
	}

	return NULL;
}

Trie_Node::~Trie_Node()
{
}
