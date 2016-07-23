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
	unsigned char _ch; //字符
	LongType _count; //字符出现的个数
	string _code; //字符对应的Huffman

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
			//使用字符的ASCII码做数组下标
			_info[i]._ch = (unsigned char)i;
		}
	}

	void Compress()
	{
		//统计字符出现的个数
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

		//构建HuffmanTree
		HuffmanTree.CreateHuffmanTree(_info, 256);

		//生成Huffman编码
		HuffmanTreeNode<charInfo>* root = HuffmanTree.GetRoot();
		string code;
		_GenerateHuffmanCode(root, code);

		//创建压缩文件并打开
		string CompressFileName = "CompressFile";
		CompressFileName += ".txt";

		FILE* fIn = fopen(CompressFileName.c_str(), "wb");
		if (fIn == NULL)
		{
			perror("open CompressFile");
			return;
		}

		fseek(fd, 0, SEEK_SET);  //重新定位文件位置指针指向文件开头，SEEK_SET表示文件开头
		int pos = 0; //ops <= 8 表示一个字符的8位
		char value = 0; //向压缩文件中存入的值

		while ((ch = fgetc(fd)) != EOF)
		{
			string code = _info[ch]._code; //获取相应字符的Huffmancode
			for (int i = 0; i < code.size(); i++)
			{
				//如果Huffmancode的某一位为1就，就存入value中，为0则直接左移value
				value <<= 1;
				if (code[i] == '1')
				{
					value |= 1;
				}
				pos++;
				if (pos == 8) //存满一个字符就将value的值存入压缩文件
				{
					fputc(value, fIn);
					pos = 0;
					value = 0;
				}
			}
		}
		if (pos != 0) //最后一个Huffmancode没有存够8位
		{
			value <<= (8 - pos);
			fputc(value, fIn);
		}

		//将HuffmanTree的叶子节点的值及相应的字符存入配置文件中，方便解压缩时重建HuffmanTree
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
				//压缩图片时会出现字符‘\0’,使用fputc函数，将字符‘\0’存入配置文件
				if (ch == '\0')
				{
					fputc(ch, fCon);
					str = ",";
				}
				else
					str += ',';

				//char *  itoa ( int value, char * str, int base );
				//将_count转换为字符串，保存在buf中，以10进制表示
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
		//打开配置文件
		FILE* fConf = fopen("ConfigFile.txt", "rb");
		if (fConf == NULL)
		{
			perror("open ConfigFile");
			return;
		}
		int i = 0;
		//从配置文件中每次读取一行，存入字符数组中
		char str[20];
		memset(str, '\0', sizeof(str));
		while (ReadLine(fConf, str, i))
		{
			if (strlen(str) || (str[1] != '\0'))
			{
				//string substr ( size_t pos, size_t n) const;
				//substr返回从第2个位置往后的字符
				unsigned char ch = str[0]; //注意类型转换将char->unsinged char
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
		//生成Huffman编码
		HuffmanTreeNode<charInfo>* root = HuffmanTree.GetRoot();
		string code;
		_GenerateHuffmanCode(root, code);

		//创建解压缩文件
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

		//读压缩文件
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
		LongType charNum = root->_weight._count;//要解析的字符的个数

		ch = fgetc(fOut);
		while (1)
		{
			//从ch最高位开始依次 & 1，判断向左走还是向右走
			--pos;
			if (ch & (1 << pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			//到叶子结点就将相应的字符存入解压缩文件
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch, fIn);
				cur = root;//查找下一个字符

				// 使用字符个数来判断解压缩是否完成，因为压缩时，最后一个字符
				// 可能存在补位，继续解压缩，会误解一些字符
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
		//递归到叶子节点就将相应Huffman编码赋给相应的叶子节点
		if (root->_left == NULL && root->_right == NULL)
		{
			_info[root->_weight._ch]._code = code;
			return;
		}
		//Huffman编码规则左0，右1
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
	cout << "压缩时间：" << end - begin << endl;
	//fc.Uncompress();
}

void TestUncompress()
{
	FileCompress fc;
	int begin = GetTickCount();
	fc.Uncompress();
	int end = GetTickCount();
	cout << "解压缩时间：" << end - begin << endl;
}