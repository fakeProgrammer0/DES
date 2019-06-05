//
// Created by fakeProgrammer0 on 2019/6/5.
//

#ifndef DES_KEYGENERATOR_H
#define DES_KEYGENERATOR_H


#include "overall.h"

#define KEY_SIZE 64
#define HALF_KEY_SIZE 28

typedef bitset<KEY_SIZE> des_key;
typedef bitset<HALF_KEY_SIZE> key_half;

class KeyGenerator
{
private:
    des_key key;
    key_half key_c;
    key_half key_d;
    const int SHL_bit_cnts[DES_ITERATION] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
    array<iter_key, DES_ITERATION> iter_keys;

    const char* trans_select_table1_file = "key_trans_select_table1.txt";
    const char* trans_select_table2_file = "key_trans_select_table2.txt";

    void trans_select1();
    void gen_iter_keys();
    void loop_left_shift(key_half& k, int n);

public:
    // KeyGenerator(unsigned long key);
    KeyGenerator(des_key key);
    ~KeyGenerator();

    array<iter_key, DES_ITERATION> get_iter_keys() const;

};


#endif //DES_KEYGENERATOR_H
