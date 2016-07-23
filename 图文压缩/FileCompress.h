#pragma once
#include<windows.h>
#include<string>
#include<stdlib.h>
#include<assert.h>
#include "HuffmanTree.h"
#pragma warning(disable:4996)

typedef unsigned long LongType;

struct charInfo
{
	unsigned char _ch; //�ַ�
	LongType _count; //�ַ����ֵĸ���
	string _code; //�ַ���Ӧ��Huffman

	charInfo(LongType count = 0)
		:_ch(0)
		, _count(count)
	{}

	bool operator != (int)
	{
		return this->_count != 0;
	}

	bool operator < (charInfo& ch)
	{
		return this->_count < ch._count;
	}

	charInfo operator + (charInfo& ch)
	{
		return charInfo(_count + ch._count);
	}

};

class FileCompress
{
public:
	FileCompress()
	{
		for (int i = 0; i < 256; i++)
		{
			//ʹ���ַ���ASCII���������±�
			_info[i]._ch = (unsigned char)i;
		}
	}

	void Compress()
	{
		//ͳ���ַ����ֵĸ���
		int ch;
		//FILE* fd = fopen("file.txt", "rb");
		FILE* fd = fopen("kkw.gif", "rb");
		//FILE* fd = fopen("test.docx", "rb");
		if (fd == NULL)
		{
			perror("open file");
			return;
		}
		while ((ch = fgetc(fd)) != EOF)
		{
			_info[ch]._count++;
		}

		//����HuffmanTree
		HuffmanTree.CreateHuffmanTree(_info, 256);

		//����Huffman����
		HuffmanTreeNode<charInfo>* root = HuffmanTree.GetRoot();
		string code;
		_GenerateHuffmanCode(root, code);

		//����ѹ���ļ�����
		string CompressFileName = "CompressFile";
		CompressFileName += ".txt";

		FILE* fIn = fopen(CompressFileName.c_str(), "wb");
		if (fIn == NULL)
		{
			perror("open CompressFile");
			return;
		}

		fseek(fd, 0, SEEK_SET);  //���¶�λ�ļ�λ��ָ��ָ���ļ���ͷ��SEEK_SET��ʾ�ļ���ͷ
		int pos = 0; //ops <= 8 ��ʾһ���ַ���8λ
		char value = 0; //��ѹ���ļ��д����ֵ

		while ((ch = fgetc(fd)) != EOF)
		{
			string code = _info[ch]._code; //��ȡ��Ӧ�ַ���Huffmancode
			for (int i = 0; i < code.size(); i++)
			{
				//���Huffmancode��ĳһλΪ1�ͣ��ʹ���value�У�Ϊ0��ֱ������value
				value <<= 1;
				if (code[i] == '1')
				{
					value |= 1;
				}
				pos++;
				if (pos == 8) //����һ���ַ��ͽ�value��ֵ����ѹ���ļ�
				{
					fputc(value, fIn);
					pos = 0;
					value = 0;
				}
			}
		}
		if (pos != 0) //���һ��Huffmancodeû�д湻8λ
		{
			value <<= (8 - pos);
			fputc(value, fIn);
		}

		//��HuffmanTree��Ҷ�ӽڵ��ֵ����Ӧ���ַ����������ļ��У������ѹ��ʱ�ؽ�HuffmanTree
		string ConfigFileName = "ConfigFile";
		ConfigFileName += ".txt";
		FILE *fCon = fopen(ConfigFileName.c_str(), "wb");
		if (fCon == NULL)
		{
			perror("open ConfigFile");
			return;
		}

		string str;
		char buf[20];

		for (int i = 0; i < 256; i++)
		{
			if (_info[i]._count > 0)
			{
				str = _info[i]._ch;
				ch = _info[i]._ch;
				//ѹ��ͼƬʱ������ַ���\0��,ʹ��fputc���������ַ���\0�����������ļ�
				if (ch == '\0')
				{
					fputc(ch, fCon);
					str = ",";
				}
				else
					str += ',';

				//char *  itoa ( int value, char * str, int base );
				//��_countת��Ϊ�ַ�����������buf�У���10���Ʊ�ʾ
				_itoa(_info[i]._count, buf, 10);
				str += buf;
				str += '\n';
				fputs(str.c_str(), fCon);
			}
		}
		fclose(fd);
		fclose(fIn);
		fclose(fCon);
	}

	bool ReadLine(FILE* fd, char* str, int i)
	{
		assert(fd);
		char ch;
		ch = fgetc(fd);
		while (!feof(fd) && ch != '\n')
		{
			str[i++] = ch;
			ch = fgetc(fd);
		}
		if (ch == '\n')
			return true;
		else
			return false;
	}

	void Uncompress()
	{
		//�������ļ�
		FILE* fConf = fopen("ConfigFile.txt", "rb");
		if (fConf == NULL)
		{
			perror("open ConfigFile");
			return;
		}
		int i = 0;
		//�������ļ���ÿ�ζ�ȡһ�У������ַ�������
		char str[20];
		memset(str, '\0', sizeof(str));
		while (ReadLine(fConf, str, i))
		{
			if (strlen(str) || (str[1] != '\0'))
			{
				//string substr ( size_t pos, size_t n) const;
				//substr���شӵ�2��λ��������ַ�
				unsigned char ch = str[0]; //ע������ת����char->unsinged char
				_info[ch]._count = atoi(str + 2);
				memset(str, '\0', sizeof(str));
				i = 0;
			}
			else
			{
				str[i++] = '\n';
			}
		}
		HuffmanTree.CreateHuffmanTree(_info, 256);
		//����Huffman����
		HuffmanTreeNode<charInfo>* root = HuffmanTree.GetRoot();
		string code;
		_GenerateHuffmanCode(root, code);

		//������ѹ���ļ�
		string UncompressFileName = "UncompressFile";
		//UncompressFileName += ".docx";
		UncompressFileName += ".gif";
		//UncompressFileName += ".txt";

		FILE* fIn = fopen(UncompressFileName.c_str(), "wb");
		if (fIn == NULL)
		{
			perror("open UncompressFile");
			return;
		}

		//��ѹ���ļ�
		FILE* fOut = fopen("CompressFile.txt", "rb");
		if (fOut == NULL)
		{
			perror("open CompressFile");
			return;
		}

		char ch = 0;
		int pos = 8;
		//HuffmanTreeNode<charInfo>* root = HuffmanTree.GetRoot();
		HuffmanTreeNode<charInfo>* cur = root;
		LongType charNum = root->_weight._count;//Ҫ�������ַ��ĸ���

		ch = fgetc(fOut);
		while (1)
		{
			//��ch���λ��ʼ���� & 1���ж������߻���������
			--pos;
			if (ch & (1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			//��Ҷ�ӽ��ͽ���Ӧ���ַ������ѹ���ļ�
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				cur = root;//������һ���ַ�

				// ʹ���ַ��������жϽ�ѹ���Ƿ���ɣ���Ϊѹ��ʱ�����һ���ַ�
				// ���ܴ��ڲ�λ��������ѹ���������һЩ�ַ�
				if (--charNum == 0)
					break;
			}

			if (pos == 0)
			{
				ch = fgetc(fOut);
				pos = 8;
			}
		}

		fclose(fIn);
		fclose(fOut);
		fclose(fConf);
	}

private:
	void _GenerateHuffmanCode(HuffmanTreeNode<charInfo>*& root, string code)
	{
		//�ݹ鵽Ҷ�ӽڵ�ͽ���ӦHuffman���븳����Ӧ��Ҷ�ӽڵ�
		if (root->_left == NULL && root->_right == NULL)
		{
			_info[root->_weight._ch]._code = code;
			return;
		}
		//Huffman���������0����1
		_GenerateHuffmanCode(root->_left, code + "0");
		_GenerateHuffmanCode(root->_right, code + "1");
	}
private:
	charInfo _info[256];
	HuffmanTree<charInfo> HuffmanTree;
};

void TestCompress()
{
	FileCompress fc;
	int begin = GetTickCount();
	fc.Compress();
	int end = GetTickCount();
	cout << "ѹ��ʱ�䣺" << end - begin << endl;
	//fc.Uncompress();
}

void TestUncompress()
{
	FileCompress fc;
	int begin = GetTickCount();
	fc.Uncompress();
	int end = GetTickCount();
	cout << "��ѹ��ʱ�䣺" << end - begin << endl;
}