#include "DES.h"

DES_codec::DES_codec(des_key key)
	: keyGen(key)
{
	init_keys();
	read_tables();
}

DES_codec::~DES_codec()
{
	release_tables();
}

void DES_codec::init_keys()
{
	const array<iter_key, DES_ITERATION>& keys = keyGen.get_iter_keys();
	auto key_ptr = keys.cbegin();
	for (auto encrypt_key_ptr = encrypt_keys.begin(); encrypt_key_ptr < encrypt_keys.end(); ++encrypt_key_ptr, ++key_ptr)
	{
		*encrypt_key_ptr = *key_ptr;
	}

	auto reverse_key_ptr = keys.crbegin();
	for (auto decrypt_key_ptr = decrypt_keys.begin(); decrypt_key_ptr < decrypt_keys.end(); ++decrypt_key_ptr, ++reverse_key_ptr)
	{
		*decrypt_key_ptr = *reverse_key_ptr;
	}
}

void DES_codec::read_tables()
{
	ifstream* IP_stream = get_data_ifstream(IP_file);
	IP_table = read_trans_table(*IP_stream);
	delete IP_stream;

	ifstream* inverse_IP_stream = get_data_ifstream(inverse_IP_file);
	inverse_IP_table = read_trans_table(*inverse_IP_stream);
	delete inverse_IP_stream;

	ifstream* E_stream = get_data_ifstream(E_file);
	E_table = read_trans_table(*E_stream);
	delete E_stream;

	ifstream* P_stream = get_data_ifstream(P_file);
	P_table = read_trans_table(*P_stream);
	delete P_stream;

	for (size_t s = 0; s < S_tables.max_size(); s++)
	{
		stringstream ss;
		ss << "S" << s + 1 << ".txt";
		ifstream* S_stream = get_data_ifstream(ss.str().c_str());
		S_tables[s] = read_trans_table(*S_stream);
		delete S_stream;
	}
}

void DES_codec::release_tables()
{
	delete IP_table;
	delete inverse_IP_table;
	delete E_table;
	delete P_table;
	for (size_t s = 0; s < S_tables.size(); s++)
		delete S_tables[s];
}

void DES_codec::codec_EBC(istream & in, ostream & out, const array<iter_key, DES_ITERATION>& keys)
{
	while (in.peek() != EOF)
	{
		des_block block_bits;
		read_bitstring(in, block_bits);
		
		des_half_block L, R;
		for (size_t i = 0; i < L.size(); i++)
			L[i] = block_bits[(*IP_table)[i]];
		for (size_t i = 0; i < R.size(); i++)
			R[i] = block_bits[(*IP_table)[i + HALF_BLOCK_SIZE]];

		/*
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			L[i] = block_bits[i];
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			R[i] = block_bits[i + HALF_BLOCK_SIZE];
		*/
		
		for (size_t idx = 0; idx < DES_ITERATION; idx++)
		{
			des_temp_block ext;
			for (size_t i = 0; i < TEMP_BLOCK_SIZE; i++)
				ext[i] = R[(*E_table)[i]];

			ext ^= keys[idx];

			des_half_block replace;
			size_t j = 0, k = 0;
			for (size_t s = 0; s < S_tables_size; s++)
			{
				size_t row = ext[j] * 2 + ext[j + 1];
				j += 2;
				size_t col = 0;
				for (size_t i = 0; i < 4; i++)
					col = (col << 1) + ext[j++];
				unsigned val = (*(S_tables[s]))[row * S_table_width + col];
				for (int i = 3; i >= 0; i--)
				{
					replace[k + i] = val & 1;
					val >>= 1;
				}
				k += 4;
			}

			des_half_block trans;
			for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
				trans[i] = replace[(*P_table)[i]];

			des_half_block L_pre = L;
			L = R;
			R = L_pre ^ trans;
		}

		des_block temp_block;
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i] = R[i];
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i + HALF_BLOCK_SIZE] = L[i];

		for (size_t i = 0; i < block_bits.size(); i++)
			block_bits[i] = temp_block[(*inverse_IP_table)[i]];
		write_bitstring(out, block_bits);
	}
}

void DES_codec::encrypt_CBC(istream& in, ostream& out)
{
	const array<iter_key, DES_ITERATION>& keys = encrypt_keys;
	des_block init_vector;

	while (in.peek() != EOF)
	{
		des_block block_bits;
		read_bitstring(in, block_bits);

		block_bits ^= init_vector;

		des_half_block L, R;
		for (size_t i = 0; i < L.size(); i++)
			L[i] = block_bits[(*IP_table)[i]];
		for (size_t i = 0; i < R.size(); i++)
			R[i] = block_bits[(*IP_table)[i + HALF_BLOCK_SIZE]];

		for (size_t idx = 0; idx < DES_ITERATION; idx++)
		{
			des_temp_block ext;
			for (size_t i = 0; i < TEMP_BLOCK_SIZE; i++)
				ext[i] = R[(*E_table)[i]];

			ext ^= keys[idx];

			des_half_block replace;
			size_t j = 0, k = 0;
			for (size_t s = 0; s < S_tables_size; s++)
			{
				size_t row = ext[j] * 2 + ext[j + 1];
				j += 2;
				size_t col = 0;
				for (size_t i = 0; i < 4; i++)
					col = (col << 1) + ext[j++];
				unsigned val = (*(S_tables[s]))[row * S_table_width + col];
				for (int i = 3; i >= 0; i--)
				{
					replace[k + i] = val & 1;
					val >>= 1;
				}
				k += 4;
			}

			des_half_block trans;
			for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
				trans[i] = replace[(*P_table)[i]];

			des_half_block L_pre = L;
			L = R;
			R = L_pre ^ trans;
		}

		des_block temp_block;
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i] = R[i];
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i + HALF_BLOCK_SIZE] = L[i];

		for (size_t i = 0; i < block_bits.size(); i++)
			block_bits[i] = temp_block[(*inverse_IP_table)[i]];
		write_bitstring(out, block_bits);
		init_vector = block_bits;
	}
}

void DES_codec::decrypt_CBC(istream& in, ostream& out)
{
	const array<iter_key, DES_ITERATION>& keys = decrypt_keys;
	des_block SV;
	des_block Next_SV;

	while (in.peek() != EOF)
	{
		des_block block_bits;
		read_bitstring(in, block_bits);

		Next_SV = block_bits;

		des_half_block L, R;
		for (size_t i = 0; i < L.size(); i++)
			L[i] = block_bits[(*IP_table)[i]];
		for (size_t i = 0; i < R.size(); i++)
			R[i] = block_bits[(*IP_table)[i + HALF_BLOCK_SIZE]];

		for (size_t idx = 0; idx < DES_ITERATION; idx++)
		{
			des_temp_block ext;
			for (size_t i = 0; i < TEMP_BLOCK_SIZE; i++)
				ext[i] = R[(*E_table)[i]];

			ext ^= keys[idx];

			des_half_block replace;
			size_t j = 0, k = 0;
			for (size_t s = 0; s < S_tables_size; s++)
			{
				size_t row = ext[j] * 2 + ext[j + 1];
				j += 2;
				size_t col = 0;
				for (size_t i = 0; i < 4; i++)
					col = (col << 1) + ext[j++];
				unsigned val = (*(S_tables[s]))[row * S_table_width + col];
				for (int i = 3; i >= 0; i--)
				{
					replace[k + i] = val & 1;
					val >>= 1;
				}
				k += 4;
			}

			des_half_block trans;
			for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
				trans[i] = replace[(*P_table)[i]];

			des_half_block L_pre = L;
			L = R;
			R = L_pre ^ trans;
		}

		des_block temp_block;
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i] = R[i];
		for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
			temp_block[i + HALF_BLOCK_SIZE] = L[i];

		for (size_t i = 0; i < block_bits.size(); i++)
			block_bits[i] = temp_block[(*inverse_IP_table)[i]];
		block_bits ^= SV;
		write_bitstring(out, block_bits);
		SV = Next_SV;
	}
}


