#pragma once

#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<cmath>

template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* m_left;
	HuffmanTreeNode<T>* m_right;
	HuffmanTreeNode<T>* m_parent;
	T m_weight;

	HuffmanTreeNode(const T& weight = T())
		:m_left(nullptr), m_right(nullptr),m_parent(nullptr), m_weight(weight)
	{}
};

template<class T>
class Compare
{
public:
	bool operator()(HuffmanTreeNode<T>* left, HuffmanTreeNode<T>* right)
	{
		return left->m_weight > right->m_weight;
	}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:m_root(nullptr)
	{}

	HuffmanTree(const std::vector<T>& vw, const T& nth)
	{
		std::priority_queue < Node*, std::vector<Node*>, Compare<T>> qw;
		for (auto& e : vw)
		{
			if(e != nth)
			qw.push(new Node(e));
		}

		while (qw.size() > 1)
		{
			Node* left = qw.top();
			qw.pop();

			Node* right = qw.top();
			qw.pop();

			Node* parent = new Node(left->m_weight + right->m_weight);
			parent->m_left = left;
			left->m_parent = parent;
			parent->m_right = right;
			right->m_parent = parent;
			qw.push(parent);
		}
		m_root = qw.top();
	}

	~HuffmanTree()
	{
		destory(m_root);
	}

	void destory(Node*& root)
	{
		if (root)
		{
			destory(root->m_left);
			destory(root->m_right);
			delete root;
			root = nullptr;
		}
	}

	Node* getRoot()
	{
		return m_root;
	}
private:
	Node* m_root;
};

