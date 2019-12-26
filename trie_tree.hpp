#pragma once
#include <string>

const int ALPHABET_SIZE = 127;


struct trie_node
{
	trie_node *children{ nullptr };
	char c{};
	bool end_of_word{};
	int count{ 0 };

	trie_node() = default;
	//children = new trie_node[ALPHABET_SIZE];
};

class trie
{
private:
	trie_node *root_;

	int size_ = 0;
public:
	trie()
	{
		root_ = new trie_node;
		root_->c = '*';
	}

	void add_word(std::string s)
	{
		auto node = root_;
		for (size_t i = 0; i < s.length(); i++)
		{
			if (node->children == nullptr)
				node->children = new trie_node[ALPHABET_SIZE];
			if (node->children[s.at(i) % ALPHABET_SIZE].c != s.at(i))
				node->children[s.at(i) % ALPHABET_SIZE].c = s.at(i);
			node = &(node->children[s.at(i) % ALPHABET_SIZE]);
		}
		node->end_of_word = true;
		node->count++;
		if (node->count == 1)
			size_++;
	}

	int has_word(std::string s)
	{
		auto node = root_;
		for (size_t i = 0; i < s.length(); i++)
		{
			if (node->children == nullptr)
				return false;
			if (node->children[s.at(i) % ALPHABET_SIZE].c == s.at(i))
				node = &(node->children[s.at(i) % ALPHABET_SIZE]);
			else
				return false;
		}
		return node->count;
	}

	int get_size()
	{
		return size_;
	}
};