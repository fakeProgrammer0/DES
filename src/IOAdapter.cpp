//
// Created by fakeProgrammer0 on 2019/6/5.
//

#include "IOAdapter.h"

// 读表, 留意 1-based index -> 0-based index
void IOAdapter::read_trans_table(trans_table &table, const char *table_file)
{
    string file(DATA_DIR);
    file += table_file;
    ifstream in(file, ios::in);
    if (!in.is_open())
    {
        cerr << "cannot open file: " << file << endl;
        exit(1);
    }

    int i;
    while (!in.eof())
    {
        in >> i;
        i--; // 下标减1，变为从0开始
        table.push_back(i);
    }
    in.close();
}

void IOAdapter::read_bits(istream &in, des_block &bits)
{
    bits.reset();
    for (size_t i = 0; i < bits.size() && !in.eof(); i++)
    {
        char c;
        in >> c;
        if (c != '0' && c != '1')
        {
            cerr << "the input bit stream has unexpected char: " << c << endl;
            exit(1);
        }
        bits[i] = c - '0';
    }
}

void IOAdapter::write_bits(ostream &out, des_block &bits)
{
    for (size_t i = 0; i < bits.size(); i++)
    {
        out << bits[i];
    }
}





