#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <functional>
#include "../Common_libs/errors.hpp"

namespace base
{

	struct DefaultComarator
	{
		template <class U>
		bool operator()(const U &lhs, const U &rhs)
		{
			return lhs < rhs;
		}
	};
} // namespace base

template <class T, class Comp = base::DefaultComarator>
class base_data
{

private:
	void DeleteNodes();

public:
	///////////////////////
	//	T mush have
	//	default, copy and moving constructors
	///////////////////////
	struct node
	{
		node(const T &init_data = T(), node *parent = nullptr);
		node(const node &that) = default;
		node(node &&that) = default;
		node &operator=(const node &that) = default;

		bool operator==(const node &that) const;
		bool operator!=(const node &that) const { return !(operator==(that)); }

		T GetData() const { return data_; }
		int GetHeight() const { return height_; }
		int DiffHeights() const;

		void CorrectHeight();

		T data_;

		int height_;
		int count_;

		node *left_;
		node *right_;
		node *parent_;
	};

	class iterator
	{
	public:
		iterator() = default;
		iterator(const iterator &) = default;
		iterator(iterator &&) = default;
		iterator &operator=(const iterator &) = default;

		int GetLeftCount() const { return left_count_; }

		const T operator*() const { return std::move(node_->GetData()); }
		const T *operator->() const { return node_; }

		void operator++();
		void operator++(int);
		void operator--();
		void operator--(int);

		//yes, I know about default iterator therefore:
		// there are equals to each other
		// iterator() == end();
		bool operator!=(const iterator &that) const { return node_ != that.node_; }
		bool operator==(const iterator &that) const { return node_ == that.node_; }

		explicit operator bool() const { return node_ != nullptr; }

	private:
		node *node_ = nullptr;
		int left_count_ = 0;

		iterator(node *init_node, int l_cnt = -1000000) : node_(init_node), left_count_(l_cnt) {}
		friend base_data<T, Comp>;
	};

	base_data() = default;
	~base_data();

	base_data(const base_data &that) { operator=(that); }
	base_data(base_data &&that) = delete;
	base_data &operator=(const base_data &);

	bool operator==(const base_data &that) const;
	bool operator!=(const base_data &that) const { return !operator==(that); }

	void insert(const T &init_node);
	void insert(std::istream &in);
	void insert(std::ifstream &in);

	void ForeachIn(const std::function<void(const node &node)> &) const;

	iterator FindMin(int num = 1) const;
	int GetCountLess(int num) const;

	iterator begin() const;
	iterator end() const;

	iterator lower_bound(const T &key) const;
	iterator upper_bound(const T &key) const;

	void InorderDump(std::ostream &out, std::string = "Dump") const;

private:
	node *main_ = nullptr;

	void BalanceTree(node *cur_node);
	node *TurnRight(node *cur_node);
	node *TurnLeft(node *cur_node);

	void InorderDumpPrivate(node *cur_node, std::ostream &out) const;
	void ForeachInPrivate(node *cur_node, const std::function<void(const node *node)> &) const;

	////////////
	//	Don't use it, if you haven't read properly this function!
	//	The aim of this - operator =
	////////////
	node *CreateSameTree(const node *init_node);
};

int operator-(const base_data<int>::iterator &last, const base_data<int>::iterator &first);

//template <class T, class Comp>
//bool operator == (const typename base_data<T, Comp>::node& lhs, const typename base_data<T, Comp>::node& rhs);

template <class T, class Comp>
base_data<T, Comp>::node::node(const T &init_data /* = T()*/, node *init_parent /*= nullptr*/)
	: data_(init_data),
	  height_(1),
	  count_(1),
	  left_(nullptr),
	  right_(nullptr),
	  parent_(init_parent)
{
}
template <class T, class Comp>
int base_data<T, Comp>::node::DiffHeights() const
{
	int left = left_ ? left_->height_
					 : 0;

	int right = right_ ? right_->height_
					   : 0;

	return left - right;
}
template <class T, class Comp>
void base_data<T, Comp>::node::CorrectHeight()
{
	int left = left_ ? left_->height_
					 : 0;

	int right = right_ ? right_->height_
					   : 0;

	int left_count = left_ ? left_->count_
						   : 0;

	int right_count = right_ ? right_->count_
							 : 0;

	height_ = left > right ? (left + 1)
						   : (right + 1);

	count_ = left_count + right_count + 1;
}
template <class T, class Comp>
bool base_data<T, Comp>::node::operator==(const node &that) const
{
	if (this == std::addressof(that))
		return true;
	if (data_ != that.data_)
		return false;

	if (left_ != that.left_ && (!left_ || !that.left_) || *left_ != *(that.left_))

		return false;

	if (right_ != that.right_ && (!right_ || !that.right_) || *right_ != *(that.right_))

		return false;

	return true;
}

template <class T, class Comp>
base_data<T, Comp>::~base_data()
{
	DeleteNodes();
}
template <class T, class Comp>
void base_data<T, Comp>::DeleteNodes()
{
	if (main_)
	{
		std::vector<node *> arr;
		for (auto iter = begin(); iter != end(); ++iter)
			arr.push_back(iter.node_);
		for (node *nd : arr)
			delete nd;
	}
}
template <class T, class Comp>
base_data<T, Comp> &base_data<T, Comp>::operator=(const base_data &that)
{
	//check calling this = this
	if (main_ == that.main_)
		return *this;

	DeleteNodes();

	main_ = CreateSameTree(that.main_);
	main_->parent_ = nullptr;

	return *this;
}
template <class T, class Comp>
typename base_data<T, Comp>::node *
base_data<T, Comp>::CreateSameTree(const node *init_node)
{
	if (init_node == nullptr)
		return nullptr;

	node *new_node = new node;
	new_node->data_ = init_node->data_;
	new_node->count_ = init_node->count_;
	new_node->height_ = init_node->height_;

	new_node->left_ = CreateSameTree(init_node->left_);
	new_node->right_ = CreateSameTree(init_node->right_);

	if (new_node->left_)
		new_node->left_->parent_ = new_node;
	if (new_node->right_)
		new_node->right_->parent_ = new_node;

	return new_node;
}
template <class T, class Comp>
bool base_data<T, Comp>::operator==(const base_data &that) const
{
	if (main_ == that.main_)
		return true;
	if (!main_ || !that.main_)
		return false;

	return (*main_ == *(that.main_));
}
template <class T, class Comp>
void base_data<T, Comp>::insert(const T &init_node)
{
	if (main_)
	{

		node *cur_node = main_;
		while (cur_node)
		{

			if (init_node == cur_node->data_)
				return;

			Comp functor;
			if (functor(init_node, cur_node->data_))
			{
				if (cur_node->left_)
				{
					cur_node = cur_node->left_;
					continue;
				}
				cur_node->left_ = new node{init_node, cur_node};
				break;
			}

			//init_node now > cur_node->data_
			if (cur_node->right_)
			{
				cur_node = cur_node->right_;
				continue;
			}

			cur_node->right_ = new node{init_node, cur_node};
			break;
		}

		BalanceTree(cur_node);
		return;
	}

	main_ = new node{init_node};
}
template <class T, class Comp>
void base_data<T, Comp>::insert(std::istream &in)
{
	T data;
	in >> data;
	if (!in.good())
		ERROR("Can't read data properly!");

	insert(data);
}
template <class T, class Comp>
void base_data<T, Comp>::insert(std::ifstream &in)
{
	T data;
	in >> data;
	if (!in.good())
		ERROR("Can't read data properly!");

	insert(data);
}

template <class T, class Comp>
void base_data<T, Comp>::BalanceTree(node *cur_node)
{
	if (!cur_node)
		return;

	switch (cur_node->DiffHeights())
	{
	case -2:
		if (cur_node->right_->DiffHeights() > 0)
			cur_node->right_ = TurnRight(cur_node->right_);
		cur_node = TurnLeft(cur_node);
		break;
	case -1:
	case 0:
	case 1:
		break;
	case 2:
		if (cur_node->left_->DiffHeights() < 0)
			cur_node->left_ = TurnLeft(cur_node->left_);
		cur_node = TurnRight(cur_node);
		break;
	default:
		WARNING("Balancing not done properly!");
		break;
	}

	cur_node->CorrectHeight();

	if (cur_node->parent_)
		BalanceTree(cur_node->parent_);
}
template <class T, class Comp>
typename base_data<T, Comp>::node *base_data<T, Comp>::TurnRight(node *cur_node)
{
	//swap nodes
	////////////////////////
	//    cur       Lcur
	//    /    ->      |
	// Lcur            cur
	////////////////////////
	node *Lcur_node = cur_node->left_;
	cur_node->left_ = Lcur_node->right_;
	Lcur_node->right_ = cur_node;

	//control their parents
	Lcur_node->parent_ = cur_node->parent_;
	cur_node->parent_ = Lcur_node;
	if (cur_node->left_)
		cur_node->left_->parent_ = cur_node;

	if (!Lcur_node->parent_)
		main_ = Lcur_node;
	else
	{
		if (Lcur_node->parent_->left_ == cur_node)
			Lcur_node->parent_->left_ = Lcur_node;
		else
			Lcur_node->parent_->right_ = Lcur_node;
	}

	cur_node->CorrectHeight();
	return Lcur_node;
}
template <class T, class Comp>
typename base_data<T, Comp>::node *base_data<T, Comp>::TurnLeft(node *cur_node)
{
	//swap nodes
	////////////////////////
	//  cur          Lcur
	//    \    ->    /
	//    Lcur     cur
	////////////////////////
	node *Rcur_node = cur_node->right_;
	cur_node->right_ = Rcur_node->left_;
	Rcur_node->left_ = cur_node;

	//control their parents
	Rcur_node->parent_ = cur_node->parent_;
	cur_node->parent_ = Rcur_node;
	if (cur_node->right_)
		cur_node->right_->parent_ = cur_node;

	if (!Rcur_node->parent_)
		main_ = Rcur_node;
	else
	{
		if (Rcur_node->parent_->left_ == cur_node)
			Rcur_node->parent_->left_ = Rcur_node;
		else
			Rcur_node->parent_->right_ = Rcur_node;
	}

	cur_node->CorrectHeight();
	return Rcur_node;
}

template <class T, class Comp>
typename base_data<T, Comp>::iterator base_data<T, Comp>::begin() const
{
	node *start = main_;
	while (start->left_)
		start = start->left_;

	return iterator{start, 0};
}
template <class T, class Comp>
typename base_data<T, Comp>::iterator base_data<T, Comp>::end() const
{
	return iterator{};
}

template <class T, class Comp>
typename base_data<T, Comp>::iterator base_data<T, Comp>::lower_bound(const T &key) const
{
	node *cur_node = main_;
	int left_count = 1;
	node *last_appreciate = nullptr;

	while (cur_node)
	{
		if (cur_node->data_ == key)
		{
			if (cur_node->left_)
				left_count += cur_node->left_->count_;
			return iterator{cur_node, left_count};
		}

		if (Comp{}(key, cur_node->data_))
		{
			last_appreciate = cur_node;
			cur_node = cur_node->left_;
		}
		else
		{
			if (cur_node->left_)
				left_count += cur_node->left_->count_;
			left_count += 1;
			cur_node = cur_node->right_;
		}
	}

	return iterator{last_appreciate, left_count};
}
template <class T, class Comp>
typename base_data<T, Comp>::iterator base_data<T, Comp>::upper_bound(const T &key) const
{
	node *cur_node = main_;
	int left_count = 0;
	node *last_appreciate = nullptr;

	while (cur_node)
	{
		if (cur_node->data_ == key)
		{
			if (cur_node->left_)
				left_count += cur_node->left_->count_;
			return iterator{cur_node, left_count + 1};
		}

		Comp functor;
		if (functor(key, cur_node->data_))
			cur_node = cur_node->left_;
		else
		{
			if (cur_node->left_)
				left_count += cur_node->left_->count_;
			left_count += 1;
			last_appreciate = cur_node;
			cur_node = cur_node->right_;
		}
	}

	return iterator{last_appreciate, left_count};
}

template <class T, class Comp>
void base_data<T, Comp>::InorderDump(std::ostream &out, std::string info /*= "Dump"*/) const
{
	out << info << ": starting\n";
	if (main_)
		InorderDumpPrivate(main_, out);
	out << std::endl
		<< info << ": ended\n";
}
template <class T, class Comp>
void base_data<T, Comp>::InorderDumpPrivate(node *cur_node, std::ostream &out) const
{
	if (cur_node->left_)
		InorderDumpPrivate(cur_node->left_, out);
	out << cur_node->data_ << ":" << cur_node->height_ << "," << cur_node->count_ << "\n";
	if (cur_node->right_)
		InorderDumpPrivate(cur_node->right_, out);
}

template <class T, class Comp>
void base_data<T, Comp>::ForeachIn(const std::function<void(const node &node)> &func) const
{
	if (main_)
		ForeachInPrivate(main_, func);
}

template <class T, class Comp>
typename base_data<T, Comp>::iterator
base_data<T, Comp>::FindMin(int num /* = 1*/) const
{
	if (num < 1 || num > main_->count_)
	{
		//WARNING("Find min can't find number out of range!");
		return end();
	}

	node *cur_node = main_;
	int left_count = 1;

	while (cur_node)
	{
		if (cur_node->left_ != nullptr)
		{
			int cur_left = cur_node->left_->count_ + left_count;

			if (cur_left == num)
			{
				//DEBUG1("I'm ending!");
				return iterator{cur_node};
			}
			if (cur_left < num)
			{
				//DEBUG1("I'm going right!");
				left_count = cur_left + 1;
				cur_node = cur_node->right_;
				continue;
			}

			//cur_left > num - 2
			//DEBUG1("I'm going left!");
			cur_node = cur_node->left_;
		}
		else
		{
			//cur_node->left_ == nullptr
			if (num == left_count)
			{
				//DEBUG1("I'm ending and left is nullptr!");
				return iterator{cur_node};
			}

			//DEBUG1("I'm going right and left is nullptr!");
			left_count++;
			cur_node = cur_node->right_;
		}
	}

	WARNING("SOMETHING DEFENETELY WENT WRONG!!!!!!");
	return iterator{nullptr};
}

template <class T, class Comp>
int base_data<T, Comp>::GetCountLess(int num) const
{
	return lower_bound(num).GetLeftCount() - 1;
}

template <class T, class Comp>
void base_data<T, Comp>::ForeachInPrivate(node *cur_node, const std::function<void(const node *node)> &func) const
{
	if (cur_node->left_)
		ForeachInPrivate(cur_node->left_, func);

	func(cur_node);

	if (cur_node->right_)
		ForeachInPrivate(cur_node->right_, func);
}

template <class T, class Comp>
void base_data<T, Comp>::iterator::operator++()
{
	if (!node_)
		ERROR("Trying to make action with undefined iterator!");
	if (node_->right_)
	{
		node_ = node_->right_;
		while (node_->left_)
			node_ = node_->left_;

		return;
	}

	while (node_->parent_)
	{
		if (node_->parent_->left_ == node_ || !node_->parent_->right_)
		{
			node_ = node_->parent_;
			return;
		}
		//now node_ == node_->parent_->right
		node_ = node_->parent_;
	}

	//node node_->parent_ == nullptr;
	node_ = nullptr;
}
template <class T, class Comp>
void base_data<T, Comp>::iterator::operator++(int)
{
	operator++();
}
template <class T, class Comp>
void base_data<T, Comp>::iterator::operator--()
{
	if (!node_)
		ERROR("Trying to make action with undefined iterator!");
	if (node_->left_)
	{
		node_ = node_->left_;
		while (node_->right_)
			node_ = node_->right_;

		return;
	}
	while (node_->parent_)
	{
		if (node_->parent_->right_ == node_ || !node_->parent_->left_)
		{
			node_ = node_->parent_;
			return;
		}
		//now node_ == node_->parent_->left
		node_ = node_->parent_;
	}

	//node node_->parent_ == nullptr;
	node_ = nullptr;
}
template <class T, class Comp>
void base_data<T, Comp>::iterator::operator--(int)
{
	operator--();
}