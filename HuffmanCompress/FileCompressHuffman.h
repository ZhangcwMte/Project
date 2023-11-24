#pragma once

#include<iostream>
#include<string>
#include<vector>

#include"HuffmanTree.hpp"

typedef unsigned char uchar;

const std::string compress_postfix = ".chz";

struct ByteInfo
{
	uchar m_ch;
	size_t m_count = 0;
	std::string m_code;

	ByteInfo(size_t count = 0)
		:m_count(count)
	{}

	ByteInfo operator+(const ByteInfo& other)const
	{
		return m_count + other.m_count;
	}

	bool operator>(const ByteInfo& other)const
	{
		return m_count > other.m_count;
	}

	bool operator!=(const ByteInfo& other)const
	{
		return m_count != other.m_count;
	}
};

class FileCompressHuffman
{
public:
	FileCompressHuffman(const std::string& file_path = "");
	void CompressFile();
	void UNCCompressFile();
	void generateHuffmanCode(HuffmanTreeNode<ByteInfo>* root);
	void headInfo(FILE* fout);
	size_t getLine(FILE* fin, std::string& str);

	inline std::string getFileName()
	{
		return m_file_path.substr(0, m_file_path.find_last_of('.'));

	}

	inline std::string getPostfix()
	{
		return m_file_path.substr(m_file_path.find_last_of('.') + 1);
	}

	inline void getPath()
	{
		std::cout << "请输入文件路径：" << std::endl;
		std::cin >> m_file_path;
	}
private:
	std::vector<ByteInfo> m_fileinfo;
	std::string m_file_path;
};