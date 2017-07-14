#pragma once
#include <string>
#include <vector>

class Trie_Node
{
private:
	char data;
	std::string file_name;
	std::vector <Trie_Node*> children;
	bool IsEndOfWord{};
	Trie_Node* link{};

public:
	Trie_Node();
	Trie_Node* findChild(char c);
	~Trie_Node();
	friend class Trie;
};

