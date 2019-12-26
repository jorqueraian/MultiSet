#pragma once
#pragma once

#include <iostream>
#include <vector>

#pragma region ItemStruct
template <class T>
struct item
{
	T key;
	int count;
	item(T k, int c = 0)
	{
		key = k;
		count = c;
	}
};
#pragma endregion 

#pragma region HashFunction

template <class K>
class hash_func
{
public:
	hash_func()
	{
	}
	void set_k(){}

	int hash_function(K key, int hash_table_size)
	{
		return 0;
	}
};

template <>
inline int hash_func<int>::hash_function(int key, int hash_table_size)
{
	return key % hash_table_size;
}

template <>
inline int hash_func<std::string>::hash_function(std::string key, int hash_table_size)
{
	auto sum = 0;
	for (size_t i = 0; i < key.size(); i++)
	{
		sum += key.at(i);
	}
	return sum % hash_table_size;
}


#pragma endregion 

#pragma region MultiSetClass

template <class T>
class multi_set
{
private:
	//linked_list<item<T>> *hash_table_;
	std::vector<item<T>*> *hash_table_;
	int hash_table_size_{ 50 };
	int size_{ 0 };

	hash_func<T> hash_func_;

	bool normal_set_{ false };
public:
	explicit multi_set(const int table_size = 50, bool normal_set = false);

	~multi_set();

	bool has(T key);

	int get_count(T key);

	bool add(T key, int c = 1);

	bool remove(T key, bool node = false);

	int get_size();

	void output_values();

	std::vector<item<T>*> *get_table(int &size);

	multi_set *set_union(multi_set<T> &set);

	multi_set *set_intersection(multi_set<T> &set);

	multi_set *set_subtraction(multi_set<T> &set);
};

template <class T>
multi_set<T>::multi_set(const int table_size, bool normal_set)
{
	hash_table_size_ = table_size;
	hash_table_ = new std::vector<item<T>*>[hash_table_size_]; //array(size = hash_table_size) of vectors
	normal_set_ = normal_set;
}

template <class T>
multi_set<T>::~multi_set()
{
	for (auto i = 0; i < hash_table_size_; i++)
	{
		for (size_t j = 0; j < hash_table_[i].size(); j++)
		{
			delete hash_table_[i][j];
		}
	}
	delete[]hash_table_;
}

template <class T>
bool multi_set<T>::has(T key)
{
	auto hash_index = hash_func_.hash_function(key, hash_table_size_);

	for (size_t i = 0; i < hash_table_[hash_index].size(); i++)
	{
		if (key == hash_table_[hash_index][i]->key)
		{
			return true;
		}
	}
	return false;
}

template <class T>
int multi_set<T>::get_count(T key)
{
	auto hash_index = hash_func_.hash_function(key, hash_table_size_);

	for (size_t i = 0; i < hash_table_[hash_index].size(); i++)
	{
		if (key == hash_table_[hash_index][i]->key)
		{
			return hash_table_[hash_index][i]->count;
		}
	}
	return 0;
}

template <class T>
bool multi_set<T>::add(T key, int c)
{
	int hash_index;
	try
	{
		hash_index = hash_func_.hash_function(key, hash_table_size_);
		if (hash_index < 0)
			return false;
	}
	catch(...)
	{
		std::cout << "hash failed!" << std::endl;
	}

	if (!(hash_table_[hash_index].empty()))
	{
		for (size_t i = 0; i < hash_table_[hash_index].size(); i++)
		{
			if (key == hash_table_[hash_index][i]->key)
			{
				if (!normal_set_)
					hash_table_[hash_index][i]->count += c;
				return true;
			}
		}
	}
	item<T> *kv_pair;
	if (!normal_set_)
		kv_pair = new item<T>(key, c);
	else
		kv_pair = new item<T>(key, -1);

	hash_table_[hash_index].push_back(kv_pair);
	size_++;
	return true;
}

template <class T>
bool multi_set<T>::remove(T key, bool node)
{
	auto hash_index = hash_func_.hash_function(key, hash_table_size_);

	for (size_t i = 0; i < hash_table_[hash_index].size(); i++)
	{
		if (key == hash_table_[hash_index][i]->key)
		{
			if (!node && hash_table_[hash_index][i]->count > 1)
				--hash_table_[hash_index][i]->count;
			else
			{
				hash_table_[hash_index].erase(hash_table_[hash_index].begin() + i);
				size_++;
			}
			return true;
		}
	}
	std::cout << "Item Not found" << std::endl;
	return false;
}

template <class T>
int multi_set<T>::get_size()
{
	return size_;
}

template <class T>
void multi_set<T>::output_values()
{
	for (auto i = 0; i < hash_table_size_; i++)
	{
		if (!(hash_table_[i].empty()))
		{
			for (size_t j = 0; j < hash_table_[i].size(); j++)
			{
				std::cout << hash_table_[i][j]->key << " ";
				if (!normal_set_)
					std::cout << hash_table_[i][j]->count;
				std::cout << std::endl;
			}
		}
	}
}

template <class T>
std::vector<item<T>*> *multi_set<T>::get_table(int &size)
{
	size = hash_table_size_;
	return hash_table_;
}

template <class T>
multi_set<T> *multi_set<T>::set_union(multi_set<T> &set)
{
	multi_set<T> *output_set = new multi_set<T>(hash_table_size_);

	for (auto i = 0; i < hash_table_size_; i++)
	{
		if (!(hash_table_[i].empty()))
		{
			for (size_t j = 0; j < hash_table_[i].size(); j++)
			{
				output_set->add(hash_table_[i][j]->key, hash_table_[i][j]->count);
			}
		}
	}

	int set_hash_table_size;
	std::vector<item<T>*> *set_hash_table = set.get_table(set_hash_table_size);
	for (int i = 0; i < set_hash_table_size; i++)
	{
		if (!(set_hash_table[i].empty()))
		{
			for (size_t j = 0; j < set_hash_table[i].size(); j++)
			{
				output_set->add(set_hash_table[i][j]->key, set_hash_table[i][j]->count);
			}
		}
	}

	return output_set;
}

template <class T>
multi_set<T> *multi_set<T>::set_intersection(multi_set<T> &set)
{
	auto*output_set = new multi_set<T>(hash_table_size_);

	for (auto i = 0; i < hash_table_size_; i++)
	{
		if (!(hash_table_[i].empty()))
		{
			for (size_t j = 0; j < hash_table_[i].size(); j++)
			{
				if (set.has(hash_table_[i][j]->key))
				{
					int set_count = set.get_count(hash_table_[i][j]->key);
					if ((set_count < hash_table_[i][j]->count && set_count != -1) || hash_table_[i][j]->count == -1)
						output_set->add(hash_table_[i][j]->key, set_count);
					else
						output_set->add(hash_table_[i][j]->key, hash_table_[i][j]->count);
				}
			}
		}
	}
	return output_set;
}

template <class T>
multi_set<T>* multi_set<T>::set_subtraction(multi_set<T>& set)
{
	multi_set<T> *output_set = new multi_set<T>(hash_table_size_);

	for (auto i = 0; i < hash_table_size_; i++)
	{
		if (!(hash_table_[i].empty()))
		{
			for (size_t j = 0; j < hash_table_[i].size(); j++)
			{
				if (set.has(hash_table_[i][j]->key))
				{
					int set_count = set.get_count(hash_table_[i][j]->key);
					if (set_count != -1 && hash_table_[i][j]->count != -1)
					{
						if (hash_table_[i][j]->count - set_count > 0)
							output_set->add(hash_table_[i][j]->key, hash_table_[i][j]->count - set_count);
					}
				}
				else
				{
					output_set->add(hash_table_[i][j]->key, hash_table_[i][j]->count);
				}
			}
		}
	}
	return output_set;
}
#pragma endregion
