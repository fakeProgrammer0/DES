//
// Created by fakeProgrammer0 on 2019/6/12.
//

#ifndef DES_DESTEST_H
#define DES_DESTEST_H

#include <stdlib.h>
#include "overall.h"
#include "DES.h"

void test_codec();
void test_rnd_codec();
void test_diff_keys();
void test_diff_messages();

void test_DES_multi_blocks_EBC();
void test_DES_multi_blocks_CBC();
void cmp_DES_EBC_CBC();
void cmp_diff_keys_EBC_CBC();

size_t count_diff_bits(const string & bitstring1, const string& bitstring2);
size_t count_diff_bits(const string & bitstring1, const string& bitstring2, bool print_count_flag);

#endif //DES_DESTEST_H
