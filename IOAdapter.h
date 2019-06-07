//
// Created by fakeProgrammer0 on 2019/6/5.
//

#ifndef DES_IOADAPTER_H
#define DES_IOADAPTER_H

#include "overall.h"

class IOAdapter {
private:
    IOAdapter()= default;

public:

    static ifstream get_data_ifstream(const char* table_file);
    static void read_trans_table(trans_table& table, ifstream& in);

    static void read_bits(istream &in, des_block &bits);
    static void write_bits(ostream &out, des_block &bits);
};


#endif //DES_IOADAPTER_H