#pragma once
#include<iostream>
#include<assert.h>
#include<vector>
using namespace std;

template<class T>
class Greater
{
public:
	Greater()
	{}

	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

template<class T>
struct Less
{
public:
	Less()
	{}

	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T, class Com>
class Heap
{
public:
	Heap()
	{}

	Heap(T *arr, int size)
	{
		assert(arr);
		assert(size > 0);
		//���ѵ����ݱ�����˳�����
		for (int i = 0; i < size; i++)
		{
			_v.push_back(arr[i]);
		}
		//�ӵ�һ����Ҷ�ӽڵ㿪ʼ����
		int root = (size - 2) / 2;
		for (; root >= 0; root--)
			_AdjustDown(root, size);
	}

	void HeapSort()
	{
		for (int i = _v.size(); i > 0; i--)
		{
			swap(_v[i - 1], _v[0]);
			int root = (i - 1) / 2 - 1;
			for (; root >= 0; root--)
				_AdjustDown(root, i - 1);
		}
	}

	void Insert(const T& x)
	{
		_v.push_back(x);
		/*int root = (_v.size() - 2) / 2;
		for (; root >= 0; root--)
		_AdjustDown(root, _v.size());*/
		if (_v.size() > 1)
			_AdjustUp(); //��Ч
	}

	void Remove()//ɾ�����ڵ�
	{
		swap(_v[0], _v[_v.size() - 1]);//�������һ���ڵ�͵�һ���ڵ�
		_v.pop_back();//ɾ�����һ���ڵ�

		if (_v.size() > 0) //��Ҫ �����������
		{
			int root = (_v.size() - 1) / 2;
			for (; root >= 0; root--)
				_AdjustDown(root, _v.size());
		}
	}

	T& Top()
	{
		return _v.front();
	}

	bool Empty()
	{
		return _v.empty();
	}

	int Size()
	{
		return _v.size();
	}

protected:
	void _AdjustDown(int root, int size) //������������µ���
	{
		int lchild = root * 2 + 1; //���ӵ��±���ڸ��ڵ���±�*2+1
		int max = lchild;
		int rchild = lchild + 1;
		while (lchild < size) //�߽��������
		{
			if (rchild < size)
			{
				//if (_v[lchild] < _v[rchild]) //ѡ�����ҽڵ��нϴ��һ��
				if (Com()(_v[rchild], _v[lchild]))
					max = rchild;
			}
			//if (_v[root] < _v[max])
			if (Com()(_v[max], _v[root]))
			{
				swap(_v[root], _v[max]);
				//���б�����Ҫ����
				root = max;
				lchild = root * 2 + 1;
				rchild = lchild + 1;
				max = lchild;//��Ҫ
			}
			else
				break;
		}
	}
	void _AdjustUp()
	{
		int root = (_v.size() - 2) / 2;
		int child = _v.size() - 1;
		//while ((root >= 0) && (_v[root] < _v[child]))
		// ��child==0ʱ�����ϵ���ɡ�����ʹ��parent���жϣ�parent����С��0
		while ((child > 0) && Com()(_v[child], _v[root]))
		{
			swap(_v[root], _v[child]);
			child = root;
			root = (child - 1) / 2;
		}
	}
private:
	vector<T> _v;
};

//void TestHeap()
//{
//	int a[] = { 10, 11, 13, 12, 16, 18, 15, 17, 14, 19 };
//	Heap<int, Greater<int>> h(a, sizeof(a) / sizeof(a[0]));
//	h.HeapSort();
//	h.Insert(20);
//	h.Remove();
//}