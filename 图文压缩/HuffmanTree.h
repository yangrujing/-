#pragma once
#include"Heap.h"
#include<iostream>
using namespace std;

template<class T>
struct HuffmanTreeNode
{
	T _weight; //Ȩֵ
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
		//ģ�ⷽ����
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
			//��ʼ����Ϊÿ���ڵ㸳Ȩֵ�������뵽С���� 
			if (arr[i] != 0)
			{
				Node *node = new Node(arr[i]);
				h.Insert(node);
			}
		}

		Node *parent = NULL;
		while (h.Size() > 1) //��Ҫ
		{
			//ѡ��������С�ڵ�
			Node *left = h.Top();
			h.Remove();
			Node *right = h.Top();
			h.Remove();
			//���㸸�ڵ��Ȩֵ����������
			parent = new Node(left->_weight + right->_weight);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			h.Insert(parent); //�����ڵ�������
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
