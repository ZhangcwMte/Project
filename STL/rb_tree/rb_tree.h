#pragma once
#include<iostream>
using namespace std;

enum Color
{
	RED,
	BLACK,
};

template<class T>
struct RBTreeNode
{
	RBTreeNode<T>* m_parent;
	RBTreeNode<T>* m_left;
	RBTreeNode<T>* m_right;
	T m_data;
	Color m_col;

	RBTreeNode(const T& data)
		:m_parent(nullptr)
		,m_left(nullptr)
		,m_right(nullptr)
		,m_data(data)
		,m_col(RED)
	{}
};

template<class T, class Ref, class Ptr>
struct rb_tree_iterator
{
    typedef RBTreeNode<T> Node;
    typedef rb_tree_iterator<T, Ref, Ptr> Self;
    
    rb_tree_iterator(Node* node)
        :m_node(node)
    {}

    rb_tree_iterator(const rb_tree_iterator<T, T&, T*>& it)
        :m_node(it.m_node)
    {}

    Ref operator*()
    {
        return m_node->m_data;
    }

    Ptr operator->()
    {
        return &m_node->m_data;
    }

    bool operator!=(const Self& it)
    {
        return m_node != it.m_node;
    }

    bool operator==(const Self& it)
    {
        return m_node == it.m_node;
    }

    Self& operator++()
    {
        if(m_node->m_right)
        {
            Node* cur = m_node->m_right;

            while(cur->m_left)
                cur = cur->m_left;

            m_node = cur;
        }
        else 
        {
            Node* cur = m_node;
            Node* parent = cur->m_parent;

            while(parent && parent->m_right == cur)
            {
                cur = parent;
                parent = parent->m_parent;
            }

            m_node = parent;
        }

        return *this;
    }

    Self operator++(int)
    {
        Self ret = *this;

        if(m_node->right)
        {
            Node* cur = m_node->m_right;

            while(cur->m_left)
                cur = cur->m_left;

            m_node = cur;
        }
        else 
        {
            Node* cur = m_node;
            Node* parent = cur->m_parent;

            while(parent && parent->m_right == cur)
            {
                cur = parent;
                parent = parent->m_parent;
            }

            m_node = parent;
        }

        return ret;
    }

    Self& operator--()
    {
        if(m_node->m_left)
        {
            Node* cur = m_node->m_left;

            while(cur->m_right)
                cur = cur->m_right;

            m_node = cur;
        }
        else 
        {
            Node* cur = m_node;
            Node* parent = cur->m_parent;

            while(parent && parent->m_left == cur)
            {
                cur = parent;
                parent = parent->m_parent;
            }

            m_node = parent;
        }

        return *this;
    }

    Self operator--(int)
    {
        Self ret = *this;

        if(m_node->m_left)
        {
            Node* cur = m_node->m_left;

            while(cur->m_right)
                cur = cur->m_right;

            m_node = cur;
        }
        else 
        {
            Node* cur = m_node;
            Node* parent = cur->m_parent;

            while(parent && parent->m_left == cur)
            {
                cur = parent;
                parent = parent->m_parent;
            }

            m_node = parent;
        }

        return ret;
    }

    Node* m_node;
};

template<class K, class T, class KeyOfT>
class rb_tree
{
	typedef RBTreeNode<T> Node;
public:
    typedef rb_tree_iterator<T, T&, T*> iterator;
    typedef rb_tree_iterator<T, const T&, const T*> const_iterator;

    iterator begin()
    {
        Node* cur = m_root;

        while(cur && cur->m_left)
        {
            cur = cur->m_left;
        }

        return iterator(cur);
    }

    iterator end()
    {
        return iterator(nullptr);
    }


    const_iterator begin()const
    {
        Node* cur = m_root;

        while(cur && cur->m_left)
        {
            cur = cur->m_left;
        }

        return iterator(cur);
    }

    const_iterator end()const
    {
        return iterator(nullptr);
    }

    pair<iterator, bool> find(const K& key)const
    { 
        if(m_root == nullptr)
            return make_pair(iterator(m_root) , false);

        KeyOfT kot;
		Node* parent = nullptr;
		Node* cur = m_root;

		while (cur)
		{
			if (kot(cur->m_data) > key)
			{
				parent = cur;
				cur = cur->m_left;
			}
			else if (kot(cur->m_data) < key)
			{
				parent = cur;
				cur = cur->m_right;
			}
			else
				return make_pair(iterator(cur) , true);
		}

        return make_pair(iterator(cur) , false);
    }

	pair<iterator, bool> insert(const T& data)
	{
        if(m_root == nullptr)
        {
            m_root = new Node(data);
            m_root->m_col = BLACK;

            return make_pair(iterator(m_root) , true);
        }

        KeyOfT kot;
		Node* parent = nullptr;
		Node* cur = m_root;

		while (cur)
		{
			if (kot(cur->m_data) > kot(data))
			{
				parent = cur;
				cur = cur->m_left;
			}
			else if (kot(cur->m_data) < kot(data))
			{
				parent = cur;
				cur = cur->m_right;
			}
			else
			{
				return make_pair(iterator(cur) , false);
			}
		}
        

		Node* new_node = new Node(data);
        cur = new_node;
		if (kot(parent->m_data) > kot(data))
			parent->m_left = cur;
		else
			parent->m_right = cur;
		cur->m_parent = parent;

		while (parent && parent->m_col == RED)
		{
			Node* grandparent = parent->m_parent;
			if (grandparent->m_left == parent)
			{
				Node* uncle = grandparent->m_right;
				if (uncle && uncle->m_col == RED)
				{
					parent->m_col = uncle->m_col = BLACK;
					grandparent->m_col = RED;

					cur = grandparent;
					parent = cur->m_parent;
				}
				else
				{
					if (parent->m_left == cur)
					{
						rotateR(grandparent);
						parent->m_col = BLACK;
						grandparent->m_col = RED;
					}
					else
					{
						rotateL(parent);
						rotateR(grandparent);
						cur->m_col = BLACK;
						grandparent->m_col = RED;
					}

					break;
				}
			}
			else
			{
				Node* uncle = grandparent->m_left;
				if (uncle && uncle->m_col == RED)
				{
					parent->m_col = uncle->m_col = BLACK;
					grandparent->m_col = RED;

					cur = grandparent;
					parent = cur->m_parent;
				}
				else
				{
					if (parent->m_right == cur)
					{
						rotateL(grandparent);
						parent->m_col = BLACK;
						grandparent->m_col = RED;
					}
					else
					{
						rotateR(parent);
						rotateL(grandparent);
						cur->m_col = BLACK;
						grandparent->m_col = RED;
					}

					break;
				}
			}
		}

		m_root->m_col = BLACK;

		return make_pair(iterator(new_node) , true);
	}

    ~rb_tree()
    {
        _destroy(m_root);

        m_root = nullptr;
    }

protected:
	void rotateL(Node* parent)
	{
		Node* subR = parent->m_right;
		Node* subRL = subR->m_left;

		parent->m_right = subRL;
		if (subRL)
			subRL->m_parent = parent;

		subR->m_left = parent;
		Node* pparent = parent->m_parent;
		if (pparent)
		{
			if (pparent->m_left == parent)
				pparent->m_left = subR;
			else
				pparent->m_right = subR;

			subR->m_parent = pparent;
		}
		else
		{
			subR->m_parent = nullptr;
			m_root = subR;
		}
		parent->m_parent = subR;

	}

	void rotateR(Node* parent)
	{
		Node* subL = parent->m_left;
		Node* subLR = subL->m_right;

		parent->m_left = subLR;
		if (subLR)
			subLR->m_parent = parent;

		subL->m_right = parent;
		Node* pparent = parent->m_parent;
		if (pparent)
		{
			if (pparent->m_left == parent)
				pparent->m_left = subL;
			else
				pparent->m_right = subL;

			subL->m_parent = pparent;
		}
		else
		{
			subL->m_parent = nullptr;
			m_root = subL;
		}
		parent->m_parent = subL;

	}

    void _destroy(Node* root)
    {
        if(root == nullptr)
            return;

        _destroy(root->m_left);
        _destroy(root->m_right);

        delete root;
    }

private:
	Node* m_root = nullptr;
};
