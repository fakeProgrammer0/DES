
#include "overall.h"

const char * DATA_DIR = "..\\data\\";

ifstream* get_data_ifstream(const char* table_file)
{
//	char file[256] = "data\\";
	char file[256] = "..\\data\\";
//	strcat_s(file, 256, table_file);
	strcat(file, table_file);
	ifstream* in = new ifstream(file, ios::in);
	if (!in->is_open())
	{
		cerr << "cannot open file: " << file << endl;
		return NULL;
	}
	return in;
}

// 读表, 留意 1-based index -> 0-based index
trans_table* read_trans_table(ifstream& in)
{
	trans_table* table = new trans_table();
	int i;
	while (!in.eof())
	{
		in >> i;
		i--; // 下标减1，变为从0开始
		table->push_back(i); 
	}
	in.close();

	return table;
}

void read_bitstring(istream &in, des_block& bits)
{
	for (size_t i = 0; i < bits.size() && !in.eof(); i++)
	{
		char c;
		in >> c;
		if (c != '0' && c != '1')
		{
			cerr << "input bitstream has unexpected char: " << c << endl;
			return;
		}
		bits[i] = c - '0';
	}
}

void write_bitstring(ostream &out, des_block & bits)
{
	for (size_t i = 0; i < bits.size(); i++)
	{
		out << bits[i];
	}
}

/*
bit_vector* transpose(const trans_table& table, const bit_vector& bits)
{
	// TODO: check size

	bit_vector* trans_bits = new bit_vector();
	for (size_t i = 0; i < table.size(); i++)
	{
		(*trans_bits)[i] = bits[table[i]];
	}
	return trans_bits;
}
*/




