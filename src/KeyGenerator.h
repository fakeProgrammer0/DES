//
// Created by fakeProgrammer0 on 2019/6/5.
//

#ifndef DES_KEYGENERATOR_H
#define DES_KEYGENERATOR_H

#include "overall.h"
#include "IOAdapter.h"

class KeyGenerator
{
private:
    des_key key;
    key_half key_c;
    key_half key_d;

    array<iter_key, DES_ITERATION> encrypt_keys;
    array<iter_key, DES_ITERATION> decrypt_keys;

    // loop left shift bit counts
    const int SHL_bit_counts[DES_ITERATION]
            = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    const char *trans_select_table1_file = "key_trans_select_table1.txt";
    const char *trans_select_table2_file = "key_trans_select_table2.txt";

    void trans_select1();

    void gen_iter_keys();

    static void loop_left_shift(key_half &k, int n);

public:
    explicit KeyGenerator(des_key key);

    const array<iter_key, DES_ITERATION> &get_encrypt_keys() const;
    const array<iter_key, DES_ITERATION> &get_decrypt_keys() const;

};

#endif //DES_KEYGENERATOR_H
