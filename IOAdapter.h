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

    static void read_trans_table(trans_table& table, const char* table_file);

//    void read_block();
//    void write_block();

    static void read_bits(istream &in, des_block &bits);
    static void write_bits(ostream &out, des_block &bits);
};


#endif //DES_IOADAPTER_H
