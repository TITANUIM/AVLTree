#pragma once

#include <math.h>

template<class K, class V>
struct AVLTreeNode
{
	K _key;
	V _value;

	AVLTreeNode<K, V>* _parent;
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;

	int _bf;		// -1/0/1

	AVLTreeNode(const K& key = K(), const V& value = V())   
		:_key(key)
		, _value(value)
		, _parent(NULL)
		, _left(NULL)
		, _right(NULL)
		, _bf(0)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

// 平衡树的插入操作
	bool Insert(const K& key, const V& value)
	{
		// 空树
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}

		// 查找插入位置
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		Node* tmp;
		if (parent->_key < key)
		{
			tmp = new Node(key, value);
			parent->_right = tmp;
			tmp->_parent = parent;
		}
		else
		{
			tmp = new Node(key, value);
			parent->_left = tmp;
			tmp->_parent = parent;
		}

		// 更新平衡因子
		bool isRotate = false;
		cur = tmp;
		parent = cur->_parent;
		while (parent)
		{
			if (parent->_left == cur)
				parent->_bf--;
			else
				parent->_bf++;

			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				cur = parent;
				parent = cur->_parent;
			}
			else // -2 2 ==>调整==>旋转
			{
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1)
					{
						_RotateL(parent);
					}
					else // -1
					{
						_RotateRL(parent);
					}
				}
				else // -2
				{
					if (cur->_bf == -1)
					{
						_RotateR(parent);
					}
					else // 1
					{
						_RotateLR(parent);
					}
				}

				isRotate = true;
				break;
			}
		}

		// 让parent链接到ppNode
		if (isRotate)
		{
			Node* ppNode = parent->_parent;
			if (ppNode == NULL)
				_root = parent;
			else if (ppNode->_key > parent->_key)
				ppNode->_left = parent;
			else
				ppNode->_right = parent;
		}

		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	Node* Find(const K& key);
	bool Remove(const K& key);

	bool IsBlance()
	{
		return _IsBlance(_root);
	}

protected:
	int _Height(Node* root)
	{
		if (root == NULL)
			return 0;

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		return left > right ? left + 1 : right + 1;
	}

// 是否平衡？
	bool _IsBlance(Node* root)
	{
		if (root == NULL)
			return true;

		int bf = _Height(root->_right) - _Height(root->_left);
		//assert(bf == root->_bf);

		if (bf != root->_bf)
		{
			cout << "平衡因子异常：" << root->_key << endl;
		}

		return bf == root->_bf && abs(bf) < 2
			&& _IsBlance(root->_left) && _IsBlance(root->_right);
	}

	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

//左单旋
	void _RotateL(Node*& parent)
	{
		Node* subR = parent->_right;
		//Node* subRR = subR->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;

		// 更新平衡因子
		parent->_bf = subR->_bf = 0;

		parent = subR;
	}

//u右单旋
	void _RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		subL->_right = parent;
		subL->_parent = parent->_parent;
		parent->_parent = subL;

		parent->_bf = subL->_bf = 0;

		parent = subL;
	}

//左右单旋
	void _RotateLR(Node*& parent)
	{
		Node* pNode = parent;
		Node* subLNode = parent->_left;
		Node* subLRNode = subLNode->_right;
		int bf = subLRNode->_bf;

		_RotateL(parent->_left);
		_RotateR(parent);

		if (bf == -1)
		{
			subLNode->_bf = 0;
			pNode->_bf = 1;
		}
		else if (bf == 1)
		{
			subLNode->_bf = -1;
			pNode->_bf = 0;
		}
		else
		{
			subLNode->_bf = 0;
			pNode->_bf = 0;
		}

		subLRNode->_bf = 0;
	}

//右左单旋
	void _RotateRL(Node*& parent)
	{
		Node* pNode = parent;
		Node* subRNode = parent->_right;
		Node* subRLNode = subRNode->_left;
		int bf = subRLNode->_bf;

		_RotateR(parent->_right);
		_RotateL(parent);

		if (bf == 1)
		{
			subRNode->_bf = 0;
			pNode->_bf = -1;
		}
		else if (bf == -1)
		{
			subRNode->_bf = 1;
			pNode->_bf = 0;
		}
		else
		{
			subRNode->_bf = 0;
			pNode->_bf = 0;
		}

		subRLNode->_bf = 0;
	}

protected:
	Node* _root;
};

void TestAVLTree()
{
	AVLTree<int, int> t;
	int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	for (size_t i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		t.Insert(a[i], i);
	}

	t.InOrder();

	cout << "t是否平衡？" << t.IsBlance() << endl;

	AVLTree<int, int> t1;
	int a1[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	for (size_t i = 0; i < sizeof(a1) / sizeof(a1[0]); ++i)
	{
		t1.Insert(a1[i], i);
	}

	t1.InOrder();

	cout << "t1是否平衡？" << t1.IsBlance() << endl;
}
