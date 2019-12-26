#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>


template <class T>
struct treap_node
{
	T key;
	int count{1};
	int random_num{};
	treap_node *left_node{ nullptr };
	treap_node *right_node{ nullptr };
	treap_node *parent_node{ nullptr };


	treap_node() = default;
	treap_node(treap_node *l, treap_node *r, treap_node *p, const T k)
	{
		left_node = l;
		right_node = r;
		parent_node = p;
		key = k;

		random_num = rand();// % 7919;
	}
};

template <class T>
class treap
{
private:
	treap_node<T> *root_{ nullptr };
	int size_ = 0;

	void sort_data(treap_node<T> *n, vector<pair<T, int>> &output)
	{
		bool added = false;
		for (size_t i = 0; i < output.size(); i++)
		{
			if (output[i].second < n->count)
			{
				output.insert(output.begin() + i, pair<T, int>(n->key, n->count));
				added = true;
				break;
			}
		}
		if (!added)
			output.push_back(pair<T, int>(n->key, n->count));


		if (n->left_node != nullptr)
			sort_data(n->left_node, output);

		if (n->right_node != nullptr)
			sort_data(n->right_node, output);
	}

public:
	treap()
	{
		std::srand(std::time(NULL));
	}
	
	void insert_node(T key_val);

	treap_node<T> *search_i(T key_val) const
	{
		//two ways, iterative or recursive
		treap_node<T> *n = root_;

		while (n != nullptr)
		{
			if (n->key > key_val)
				n = n->left_node;
			else if (n->key < key_val)
				n = n->right_node;
			else
				return n;
		}
		return nullptr;
	}

	void rotate_right(treap_node<T> *rotate_root)
	{
		treap_node<T> *left = rotate_root->left_node;
		treap_node<T> *right = rotate_root->right_node;
		treap_node<T> *left_left;
		treap_node<T> *left_right;
		treap_node<T> *right_left;
		treap_node<T> *right_right;

		if (left == nullptr)
		{
			left_left = nullptr;
			left_right = nullptr;
		}
		else
		{
			left_left = left->left_node;
			left_right = left->right_node;
		}

		if (right == nullptr)
		{
			right_left = nullptr;
			right_right = nullptr;
		}
		else
		{
			right_left = right->left_node;
			right_right = right->right_node;
		}

		left->parent_node = rotate_root->parent_node;
		if (rotate_root == root_)
			root_ = left;
		else if (rotate_root->parent_node->left_node == rotate_root)
			rotate_root->parent_node->left_node = left;
		else
			rotate_root->parent_node->right_node = left;

		left->right_node = rotate_root;
		rotate_root->parent_node = left;

		if (left_right != nullptr)
		{
			rotate_root->left_node = left_right;
			left_right->parent_node = rotate_root;
		}
		else
		{
			rotate_root->left_node = nullptr;
		}
	}

	void rotate_left(treap_node<T> *rotate_root)
	{
		treap_node<T> *left = rotate_root->left_node;
		treap_node<T> *right = rotate_root->right_node;
		treap_node<T> *left_left;
		treap_node<T> *left_right;
		treap_node<T> *right_left;
		treap_node<T> *right_right;

		if (left == nullptr)
		{
			left_left = nullptr;
			left_right = nullptr;
		}
		else
		{
			left_left = left->left_node;
			left_right = left->right_node;
		}

		if (right == nullptr)
		{
			right_left = nullptr;
			right_right = nullptr;
		}
		else
		{
			right_left = right->left_node;
			right_right = right->right_node;
		}

		right->parent_node = rotate_root->parent_node;
		if (rotate_root == root_)
			root_ = right;
		else if (rotate_root->parent_node->left_node == rotate_root)
			rotate_root->parent_node->left_node = right;
		else
			rotate_root->parent_node->right_node = right;

		right->left_node = rotate_root;
		rotate_root->parent_node = right;



		if (right_left != nullptr)
		{
			rotate_root->right_node = right_left;
			right_left->parent_node = rotate_root;
		}
		else
		{
			rotate_root->right_node = nullptr;
		}
	}

	int get_size()
	{
		return size_;
	}

	vector<pair<T, int>> sort_data()
	{
		vector<pair<T, int>> output;
		auto*n = root_;
		sort_data(root_, output);
		return output;
	}

	static void print_node(treap_node<T> *n)
	{
		std::cout << n->key << std::endl;

		if (n->left_node != nullptr)
			print_node(n->left_node);

		if (n->right_node != nullptr)
			print_node(n->right_node);
	}

	void delete_node(treap_node<T>* node)
	{
		int left_child_i;
		if (node == root_)
			left_child_i = 2;
		else if (node->parent->left_child == node)
			left_child_i = 1;//== true
		else
			left_child_i = 0;
		//const auto left_child = node->parent->left_child == node;

		if (node->left_child == nullptr && node->right_child == nullptr)
		{ //no child
			//here we disconnect the parent
			if (left_child_i == 2)
				root_ = nullptr;
			else if (left_child_i == 1)
				node->parent->left_child = nullptr;
			else
				node->parent->right_child = nullptr;
		}
		else if (!(node->left_child != nullptr && node->right_child != nullptr))
		{
			if (left_child_i == 2)
			{
				if (node->left_child != nullptr)
					root_ = node->left_child;
				else
					root_ = node->right_child;
				node->parent = nullptr;
			}
			else if (node->left_child != nullptr)
			{

				if (left_child_i == 1)
					node->parent->left_child = node->left_child;
				else
					node->parent->right_child = node->left_child;
				node->left_child->parent = node->parent;
			}
			else
			{
				if (left_child_i == 1)
					node->parent->left_child = node->right_child;
				else
					node->parent->right_child = node->right_child;
				node->right_child->parent = node->parent;
			}
		}
		else
		{//we need to replace the node we are deleting,

			const auto min = tree_minimum(node->right_child);
			if (left_child_i == 2)
			{
				if (min == node->right_child)
				{
					root_ = min;
					min->parent = nullptr;
					min->left_child = node->left_child;
					min->left_child->parent = min;
				}
				else
				{
					//patch up hole where the min is
					min->parent->left_child = min->right_child;

					if (min->right_child != nullptr)
						min->right_child->parent = min->parent;

					//point min to parent
					min->parent = nullptr;
					root_ = min;

					//give min children
					min->left_child = node->left_child;
					min->right_child = node->right_child;
					node->right_child->parent = min;
					node->left_child->parent = min;
				}
			}
			else if (left_child_i == 1)
			{
				if (min == node->right_child)
				{
					node->parent->left_child = min;
					min->parent = node->parent;
					min->left_child = node->left_child;
					min->left_child->parent = min;
				}
				else
				{
					//patch up hole where the min is
					min->parent->left_child = min->right_child;

					if (min->right_child != nullptr)
						min->right_child->parent = min->parent;

					//point min to parent
					min->parent = node->parent;
					node->parent->left_child = min;

					//give min children
					min->left_child = node->left_child;
					min->right_child = node->right_child;
					node->right_child->parent = min;
					node->left_child->parent = min;
				}
			}
			else
			{
				if (min == node->right_child)
				{
					node->parent->right_child = min;
					min->parent = node->parent;
					min->left_child = node->left_child;
					min->left_child->parent = min;
				}
				else
				{
					//patch up hole where the min is
					min->parent->left_child = min->right_child;

					if (min->right_child != nullptr)
						min->right_child->parent = min->parent;

					//point min to parent
					min->parent = node->parent;
					node->parent->right_child = min;

					//give min children
					min->left_child = node->left_child;
					min->right_child = node->right_child;
					node->right_child->parent = min;
					node->left_child->parent = min;
				}
			}
		}

		delete node;
	}

	treap_node<T>* tree_minimum(treap_node<T>* node)
	{
		//gives node to delete, search lowest val; starting with the root given
		treap_node<T> *min_val;
		for (min_val = node; min_val->left_child != nullptr;
			min_val = min_val->left_child) {
		}
		return min_val;
	}

};

template <>
inline void treap<std::string>::insert_node(std::string key_val)
{
	treap_node<std::string> *parent = nullptr;
	auto temp = root_;

	auto n = new treap_node<std::string>(nullptr, nullptr, nullptr, key_val);
	while (temp != nullptr)
	{
		//empty tree root is null so yea

		if (temp->key == n->key)
		{
			delete n;
			++temp->count;
			return;
		}

		parent = temp;

		bool n_less_temp = true;

		for (size_t i = 0; i < n->key.length() && i < temp->key.length(); i++)
		{
			if (n->key.at(i) == temp->key.at(i))
			{
				continue;
			}
			n_less_temp = (n->key.at(i) < temp->key.at(i));
			break;
		}

		if (n_less_temp)
			temp = temp->left_node;
		else
			temp = temp->right_node;
	}

	if (parent == nullptr)
	{
		root_ = n;
		return;
	}

	bool n_less_p = true;

	for (size_t i = 0; i < n->key.length() && i < parent->key.length(); i++)
	{
		if (n->key.at(i) == parent->key.at(i))
		{
			continue;
		}
		n_less_p = (n->key.at(i) < parent->key.at(i));
		break;
	}

	if (n_less_p)
	{
		parent->left_node = n;
		n->parent_node = parent;
	}
	else
	{
		parent->right_node = n;
		n->parent_node = parent;
	}

	if (n == n->parent_node->left_node)
	{
		if (n->parent_node->random_num > n->random_num)
		{
			rotate_right(n->parent_node);
		}
	}
	else
	{
		if (n->parent_node->random_num > n->random_num)
		{
			rotate_left(n->parent_node);
		}
	}
	size_++;
}

template<class T>
inline void treap<T>::insert_node(T key_val)
{
	treap_node<T> *parent = nullptr;
	auto*temp = root_;

	auto*n = new treap_node<T>(nullptr, nullptr, nullptr, key_val);
	while (temp != nullptr)
	{
		//empty tree root is null so yea

		if (temp->key == n->key)
		{
			delete n;
			++temp->count;
			return;
		}

		parent = temp;
		if (n->key < temp->key)
			temp = temp->left_node;
		else
			temp = temp->right_node;
	}

	if (parent == nullptr)
	{
		root_ = n;
		return;
	}
	else if (n->key < parent->key)
	{
		parent->left_node = n;
		n->parent_node = parent;
	}
	else
	{
		parent->right_node = n;
		n->parent_node = parent;
	}

	if (n == n->parent_node->left_node)
	{
		if (n->parent_node->random_num > n->random_num)
		{
			rotate_right(n->parent_node);
		}
	}
	else
	{
		if (n->parent_node->random_num > n->random_num)
		{
			rotate_left(n->parent_node);
		}
	}
	size_++;
}
