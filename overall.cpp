
#include "overall.h"

const char * DATA_DIR = "..\\data\\";

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


