//
// Created by fakeProgrammer0 on 2019/6/5.
//

#include "KeyGenerator.h"

KeyGenerator::KeyGenerator(des_key key) :
        key(key)
{
    trans_select1();
    gen_iter_keys();
}

void KeyGenerator::trans_select1()
{
    trans_table trans_select_table1;
    IOAdapter::read_trans_table(trans_select_table1, trans_select_table1_file);

    int j = 0;
    for (int i = 0; i < HALF_KEY_LEN; i++, j++)
        key_c[i] = key[trans_select_table1[j]];

    for (int i = 0; i < HALF_KEY_LEN; i++)
        key_d[i] = key[trans_select_table1[j]];

    /*
    for (size_t i = 0; i < key.size(); i++)
    {
        cout << key[i] << " ";
        if ((i + 1) % 8 == 0)
            cout << endl;
    }
    cout << endl;

    for (size_t i = 0; i < key_c.size(); i++)
    {
        cout << key_c[i] << " ";
        if ((i + 1) % 7 == 0)
            cout << endl;
    }
    cout << endl;

    for (size_t i = 0; i < key_d.size(); i++)
    {
        cout << key_d[i] << " ";
        if ((i + 1) % 7 == 0)
            cout << endl;
    }
    cout << endl;
    */
}

void KeyGenerator::loop_left_shift(key_half &k, int n)
{
    //k = k << n | k >> (HALF_KEY_LEN - n);
    k = k >> n | k << (HALF_KEY_LEN - n); // little endian 的循环右移相当于big endian的循环左移
}

void KeyGenerator::gen_iter_keys()
{
    trans_table trans_select_table2;
    IOAdapter::read_trans_table(trans_select_table2, trans_select_table2_file);

    /*
    for(size_t idx = 0; idx < DES_ITERATION; idx++)
        cout << idx << "\t" << SHL_bit_counts[idx] << endl;
    cout << endl;
    */

    for (size_t i = 0; i < DES_ITERATION; i++)
    {
        // cout << "key_c: " << key_c << endl;
        // cout << "key_d: " << key_d << endl << endl;

        loop_left_shift(key_c, SHL_bit_counts[i]);
        loop_left_shift(key_d, SHL_bit_counts[i]);

        iter_key& k = encrypt_keys[i];

        size_t j = 0;
        for (; j < ITER_KEY_LEN / 2; j++)
            k[j] = key_c[trans_select_table2[j]];
        for (; j < ITER_KEY_LEN; j++)
            k[j] = key_d[trans_select_table2[j] - HALF_KEY_LEN];

        decrypt_keys[DES_ITERATION - 1 - i] = k;

        /*
        cout << "key_c: " << endl;
        for (size_t idx = 0; idx < key_c.size(); idx++)
        {
            cout << key_c[idx] << " ";
            if ((idx + 1) % 7 == 0)
                cout << endl;
        }
        cout << endl;

        cout << "key_d: " << endl;
        for (size_t idx = 0; idx < key_d.size(); idx++)
        {
            cout << key_d[idx] << " ";
            if ((idx + 1) % 7 == 0)
                cout << endl;
        }
        cout << endl;

        cout << "key [" << i << "]: " << endl;
        for (size_t idx = 0; idx < k.size(); idx++)
        {
            cout << k[idx] << " ";
            if ((idx + 1) % 8 == 0)
                cout << endl;
        }
        cout << endl;
        */
    }
}

const array<iter_key, DES_ITERATION> &KeyGenerator::get_encrypt_keys() const
{
    return encrypt_keys;
}

const array<iter_key, DES_ITERATION> &KeyGenerator::get_decrypt_keys() const
{
    return decrypt_keys;
}

