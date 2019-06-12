#include "DES.h"

DES_Codec::DES_Codec(des_key key)
        : keyGen(key)
{
    read_tables();
}

void DES_Codec::read_tables()
{
    IOAdapter::read_trans_table(IP_table, IP_file);
    IOAdapter::read_trans_table(inverse_IP_table, inverse_IP_file);
    IOAdapter::read_trans_table(E_table, E_file);
    IOAdapter::read_trans_table(P_table, P_file);

    for (size_t s = 0; s < S_tables.max_size(); s++)
    {
        stringstream ss;
        ss << "S" << s + 1 << ".txt";
        IOAdapter::read_trans_table(S_tables[s], ss.str().c_str());
    }
}

void DES_Codec::codec_block(des_block& block_bits, const array<iter_key, DES_ITERATION> &keys)
{
    des_half_block L, R;
    for (size_t i = 0; i < L.size(); i++)
        L[i] = block_bits[IP_table[i]];
    for (size_t i = 0; i < R.size(); i++)
        R[i] = block_bits[IP_table[i + HALF_BLOCK_SIZE]];

    for (size_t idx = 0; idx < DES_ITERATION; idx++)
    {
        des_temp_block ext;
        for (size_t i = 0; i < TEMP_BLOCK_SIZE; i++)
            ext[i] = R[E_table[i]];

        ext ^= keys[idx];

        des_half_block replace;
        size_t j = 0, k = 0;
        for (size_t s = 0; s < S_TABLES_COUNT; s++)
        {
            size_t row = ext[j] * 2 + ext[j + 1];
            j += 2;
            size_t col = 0;
            for (size_t i = 0; i < 4; i++)
                col = (col << 1) + ext[j++];
            unsigned val = (S_tables[s])[row * S_table_width + col];
            for (int i = 3; i >= 0; i--)
            {
                replace[k + i] = val & 1;
                val >>= 1;
            }
            k += 4;
        }

        des_half_block trans;
        for (size_t i = 0; i < HALF_BLOCK_SIZE; i++)
            trans[i] = replace[P_table[i]];

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
        block_bits[i] = temp_block[inverse_IP_table[i]];
}

void DES_Codec::codec_EBC(istream &in, ostream &out, const array<iter_key, DES_ITERATION> &keys)
{
    while (in.peek() != EOF)
    {
        des_block block_bits;
        IOAdapter::read_bits(in, block_bits);
        
        codec_block(block_bits, keys);
        
        IOAdapter::write_bits(out, block_bits);
    }
}

void DES_Codec::encrypt_CBC(istream &in, ostream &out)
{
    const array<iter_key, DES_ITERATION> &keys = keyGen.get_encrypt_keys();
    des_block init_vector;

    while (in.peek() != EOF)
    {
        des_block block_bits;
        IOAdapter::read_bits(in, block_bits);

        block_bits ^= init_vector;
        codec_block(block_bits, keys);
        init_vector = block_bits;

        IOAdapter::write_bits(out, block_bits);
    }
}

void DES_Codec::decrypt_CBC(istream &in, ostream &out)
{
    const array<iter_key, DES_ITERATION> &keys = keyGen.get_decrypt_keys();
    des_block SV, Next_SV;

    while (in.peek() != EOF)
    {
        des_block block_bits;
        IOAdapter::read_bits(in, block_bits);

        Next_SV = block_bits;
        codec_block(block_bits, keys);
        block_bits ^= SV;
        SV = Next_SV;

        IOAdapter::write_bits(out, block_bits);
    }
}


