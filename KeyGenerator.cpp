//
// Created by fakeProgrammer0 on 2019/6/5.
//

#include "KeyGenerator.h"

/*
KeyGenerator::KeyGenerator(unsigned long key)
{
	this->key = key;
	trans_select1();
	gen_iter_keys();
}
*/

KeyGenerator::KeyGenerator(des_key key)
{
    this->key = key;
    trans_select1();
    gen_iter_keys();
}

KeyGenerator::~KeyGenerator()
{

}


void KeyGenerator::trans_select1()
{
    ifstream* in_table1 = get_data_ifstream(trans_select_table1_file);
    trans_table* trans_select_table1 = read_trans_table(*in_table1);
    delete in_table1;

    for (int i = 0; i < HALF_KEY_SIZE; i++)
    {
        key_c[i] = key[(*trans_select_table1)[i]];
    }

    for (int i = 0; i < HALF_KEY_SIZE; i++)
    {
        key_d[i] = key[(*trans_select_table1)[i + HALF_KEY_SIZE]];
    }

    delete trans_select_table1;

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

void KeyGenerator::loop_left_shift(key_half& k, int n)
{
    //k = k << n | k >> (HALF_KEY_SIZE - n);
    k = k >> n | k << (HALF_KEY_SIZE - n); // little endian 的循环右移相当于big endian的循环左移
}

void KeyGenerator::gen_iter_keys()
{
    ifstream* in_table2 = get_data_ifstream(trans_select_table2_file);
    trans_table* trans_select_table2 = read_trans_table(*in_table2);
    delete in_table2;

    /*
    for(size_t idx = 0; idx < DES_ITERATION; idx++)
        cout << idx << "\t" << SHL_bit_cnts[idx] << endl;
    cout << endl;
    */

    for (size_t i = 0; i < DES_ITERATION; i++)
    {
        // cout << "key_c: " << key_c << endl;
        // cout << "key_d: " << key_d << endl << endl;

        loop_left_shift(key_c, SHL_bit_cnts[i]);
        loop_left_shift(key_d, SHL_bit_cnts[i]);

        iter_key& k = iter_keys[i];

        size_t j = 0;
        for (; j < ITER_KEY_LEN / 2; j++)
            k[j] = key_c[(*trans_select_table2)[j]];
        for (; j < ITER_KEY_LEN; j++)
            k[j] = key_d[(*trans_select_table2)[j] - HALF_KEY_SIZE];

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
    delete trans_select_table2;
}

array<iter_key, DES_ITERATION> KeyGenerator::get_iter_keys() const
{
    return iter_keys;
}




