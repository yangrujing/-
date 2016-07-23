#pragma once
#include"Heap.h"
#include<iostream>
using namespace std;

template<class T>
struct HuffmanTreeNode
{
	T _weight; //权值
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;

	HuffmanTreeNode(const T& weight)
		:_weight(weight)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{}
};

template <class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	~HuffmanTree()
	{
		destory(_root);
	}

	void CreateHuffmanTree(T* arr, int size)
	{
		//模拟方函数
		struct compare
		{
			bool operator()(Node*& n1, Node*& n2)
			{
				return n1->_weight <  n2->_weight;
			}
		};

		Heap<Node*, compare> h;
		for (int i = 0; i < size; i++)
		{
			//初始化，为每个节点赋权值，并插入到小堆中 
			if (arr[i] != 0)
			{
				Node *node = new Node(arr[i]);
				h.Insert(node);
			}
		}

		Node *parent = NULL;
		while (h.Size() > 1) //重要
		{
			//选出两个最小节点
			Node *left = h.Top();
			h.Remove();
			Node *right = h.Top();
			h.Remove();
			//计算父节点的权值，并链起来
			parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			h.Insert(parent); //将父节点插入堆中
		}
		_root = parent;
	}

	Node* GetRoot()
	{
		return _root;
	}
private:
	void destory(Node *&root)
	{
		if (root)
		{
			destory(root->_left);
			destory(root->_right);
			delete root;
			root = NULL;
		}
	}
private:
	Node* _root;
};

//void Test()
//{
//	int arr[] = { 0, 1, 2, 3, 4 };
//	HuffmanTree<int> HuffmanTree;
//	HuffmanTree.CreateHuffmanTree(arr, (sizeof(arr) / sizeof(arr[0])));
//}
