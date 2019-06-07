#pragma once

#ifndef DES_OVERALL_H
#define DES_OVERALL_H

/**
 * include global libs
 */

#include <bitset>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using std::bitset;
using std::vector;
using std::array;
using std::string;
using std::ios;
using std::ifstream;
using std::istream;
using std::ostream;
using std::stringstream;

using std::cin;
using std::cerr;
using std::cout;
using std::endl;

/**
 * define global variables & configurations
 */

#define DES_ITERATION 16 // the iteration of DES encryption / decryption

// keys varying in different lengths
#define KEY_LEN 64 // the length of input key
#define HALF_KEY_LEN 28 // c, d
#define ITER_KEY_LEN 48 // the length of key used in each DES iteration

typedef bitset<KEY_LEN> des_key;
typedef bitset<HALF_KEY_LEN> key_half;
typedef bitset<ITER_KEY_LEN> iter_key;

// the block size used in DES
#define BLOCK_SIZE 64
#define HALF_BLOCK_SIZE 32
#define TEMP_BLOCK_SIZE 48

typedef bitset<BLOCK_SIZE> des_block;
typedef bitset<HALF_BLOCK_SIZE> des_half_block;
typedef bitset<TEMP_BLOCK_SIZE> des_temp_block;

// data: transpose tables & select boxes
extern const char * DATA_DIR;

#define S_tables_size 8

typedef vector<int> trans_table;

#endif
