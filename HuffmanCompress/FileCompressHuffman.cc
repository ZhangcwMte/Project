#define _CRT_SECURE_NO_WARNINGS 

#include"FileCompressHuffman.h"

FileCompressHuffman::FileCompressHuffman(const std::string& file_path)
	:m_file_path(file_path)
{
	m_fileinfo.resize(256);
	for (int i = 0; i < 256; i++)
	{
		m_fileinfo[i].m_ch = i;
	}
}

void FileCompressHuffman::CompressFile()
{
	std::string file_name = getFileName();
	std::string postfix = getPostfix();
	FILE* fin = fopen(m_file_path.c_str(), "rb");
	if (fin == nullptr)
	{
		std::cout << "The file failed to open!" << std::endl;
		exit(1);
	}

	uchar buffer[1024];

	while (1)
	{
		size_t size = fread(buffer, 1, 1024, fin);
		if (size == 0)break;

		for (size_t i = 0; i < size; i++)
		{
			m_fileinfo[buffer[i]].m_count++;
		}
	}

	HuffmanTree<ByteInfo> ht(m_fileinfo, ByteInfo());

	generateHuffmanCode(ht.getRoot());

	std::string filecs_path = file_name + compress_postfix;
	FILE* fout = fopen(filecs_path.c_str(), "wb");

	headInfo(fout);

	fseek(fin, 0, SEEK_SET);

	char bits = 0;
	int bit_count = 0;
	while (1)
	{
		size_t size = fread(buffer, 1, 1024, fin);
		if (size == 0)break;

		for (size_t i = 0; i < size; i++)
		{
			std::string& code = m_fileinfo[buffer[i]].m_code;

			for (size_t j = 0; j < code.size(); j++)
			{
				bits <<= 1;
				if (code[j] == '1')
					bits |= 1;
				bit_count++;

				if (bit_count == 8)
				{
					fputc(bits, fout);
					//bits = 0;
					bit_count = 0;
				}
			}
		}
	}
	if (bit_count > 0)
	{
		bits <<= (8 - bit_count);
		fputc(bits, fout);
	}

	std::cout << "The file was compressed successfully!" << std::endl;

	fclose(fin);
	fclose(fout);
}
void FileCompressHuffman::UNCCompressFile()
{
	FILE* fin = fopen(m_file_path.c_str(), "rb");
	if (fin == nullptr)
	{
		std::cout << "The file failed to open!" << std::endl;
		exit(1);
	}

	std::string file_name = getFileName() + '.';
	std::string str;
	getLine(fin, str);
	file_name += str;

	getLine(fin, str);
	size_t head_lines = atoi(str.c_str());

	for (size_t i = 0; i < head_lines; i++)
	{
		getLine(fin, str);
		if (str == "")
		{
			getLine(fin, str);
			std::string ent = "\n";
			str = ent + str;
		}
		m_fileinfo[(uchar)str[0]].m_ch = (uchar)str[0];
		m_fileinfo[(uchar)str[0]].m_count = atoi(str.c_str() + 2);
	}

	HuffmanTree<ByteInfo> ht(m_fileinfo, ByteInfo());

	FILE* fout = fopen(file_name.c_str(), "wb");

	uchar buffer[1024];
	HuffmanTreeNode<ByteInfo>* cur = ht.getRoot();
	size_t file_size = 0;
	while (1)
	{
		size_t size = fread(buffer, 1, 1024, fin);
		if (size == 0)break;

		for (size_t i = 0; i < size; i++)
		{
			char ch = buffer[i];

			for (size_t j = 0; j < 8; j++)
			{
				if (ch >> 7 & 1)
					cur = cur->m_right;
				else
					cur = cur->m_left;

				ch <<= 1;

				if (cur->m_left == nullptr && cur->m_right == nullptr)
				{
					fputc(cur->m_weight.m_ch, fout);
					cur = ht.getRoot();
					file_size++;
					if (file_size == cur->m_weight.m_count)break;
				}
			}
		}
	}

	std::cout << "The file was decompressed successfully!" << std::endl;

	fclose(fin);
	fclose(fout);
}

void FileCompressHuffman::generateHuffmanCode(HuffmanTreeNode<ByteInfo>* root)
{
	if (root == nullptr)return;

	generateHuffmanCode(root->m_left);
	generateHuffmanCode(root->m_right);

	if (root->m_left == nullptr && root->m_right == nullptr)
	{
		std::string& code = m_fileinfo[root->m_weight.m_ch].m_code;
		HuffmanTreeNode<ByteInfo>* cur = root;
		HuffmanTreeNode<ByteInfo>* parent = cur->m_parent;
		while (parent)
		{
			if (cur == parent->m_left)
				code += '0';
			else
				code += '1';

			cur = parent;
			parent = cur->m_parent;
		}
		std::reverse(code.begin(), code.end());
	}
}

void FileCompressHuffman::headInfo(FILE* fout)
{
	std::string head_info = getPostfix();
	head_info += '\n';
	size_t head_lines = 0;
	std::string ch_info;

	for (auto& it : m_fileinfo)
	{
		if (it.m_count == 0)continue;

		ch_info += it.m_ch;
		ch_info += ':';
		ch_info += std::to_string(it.m_count);
		ch_info += '\n';

		head_lines++;
	}

	head_info += std::to_string(head_lines);
	head_info += '\n';

	fwrite(head_info.c_str(), 1, head_info.size(), fout);
	fwrite(ch_info.c_str(), 1, ch_info.size(), fout);
}

size_t FileCompressHuffman::getLine(FILE* fin, std::string& str)
{
	size_t count = 0;
	str = "";
	while (!feof(fin))
	{
		char ch = fgetc(fin);
		if (ch == '\n')break;

		str += ch;
		count++;
	}

	return count;
}



