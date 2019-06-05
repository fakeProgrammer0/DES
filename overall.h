#pragma once

#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <array>
#include <sstream>

using std::vector;
using std::array;
using std::bitset;
using std::string;
using std::ifstream;
using std::ios;
using std::istream;
using std::ostream;
using std::stringstream;

using std::cin;
using std::cerr;
using std::cout;
using std::endl;


#define DES_ITERATION 16
#define ITER_KEY_LEN 48
#define BLOCK_SIZE 64
#define HALF_BLOCK_SIZE 32
#define TEMP_BLOCK_SIZE 48

typedef vector<int> trans_table;
typedef vector<bool> bit_vector;
typedef bitset<ITER_KEY_LEN> iter_key;
typedef bitset<BLOCK_SIZE> des_block;
typedef bitset<HALF_BLOCK_SIZE> des_half_block;
typedef bitset<TEMP_BLOCK_SIZE> des_temp_block;

trans_table* read_trans_table(ifstream& in);
ifstream* get_data_ifstream(const char*);

void read_bitstring(istream & in, des_block & bits);
void write_bitstring(ostream & out, des_block & bits);
