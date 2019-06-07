#pragma once

#ifndef DES
#define DES

#include "overall.h"
#include "KeyGenerator.h"

class DES_codec
{
private:
	KeyGenerator keyGen;
	array<iter_key, DES_ITERATION> encrypt_keys;
	array<iter_key, DES_ITERATION> decrypt_keys;

	const char* IP_file = "IP.txt";
	const char* inverse_IP_file = "IP-1.txt";
	const char* E_file = "E.txt";
	const char* P_file = "P.txt";

	trans_table* IP_table;
	trans_table* inverse_IP_table;
	trans_table* E_table;
	trans_table* P_table;
	array<trans_table*, S_tables_size> S_tables;

	// static const size_t S_tables_size = 8;
	static const size_t S_table_width = 16;
	static const size_t S_table_height = 4;
	static const size_t S_table_index_size = 6;

	void init_keys();
	void read_tables();
	void release_tables();
	void codec_EBC(istream&in, ostream& out, const array<iter_key, DES_ITERATION>& keys);

public:
	DES_codec(des_key key);
	~DES_codec();

	void encrypt_EBC(istream& in, ostream& out)
	{
		codec_EBC(in, out, encrypt_keys);
	}

	void decrypt_EBC(istream& in, ostream& out) 
	{
		codec_EBC(in, out, decrypt_keys);
	}

	void encrypt_CBC(istream& in, ostream& out);
	void decrypt_CBC(istream& in, ostream& out);
};


#endif // ! DES


