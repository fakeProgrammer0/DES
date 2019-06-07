
#include <stdlib.h>
#include <random>

#include "overall.h"
#include "DES.h"

using std::default_random_engine;
using std::uniform_int_distribution;

void test_codec();
void test_rnd_codec();
void test_diff_keys();
void test_diff_messages();

void test_DES_EBC_multi_blocks();
void test_DES_CBC_multi_blocks();
void compare_DES_EBC_CBC();

size_t count_diff_bits(const string & bitstring1, const string& bitstring2);

size_t count_diff_bits(const string & bitstring1, const string& bitstring2)
{
    size_t count = 0;
    for (size_t i = 0; i < bitstring1.size() && i < bitstring2.size(); i++)
        if (bitstring1[i] != bitstring2[i])
            count++;
    //cout << "different bit count(s): " << count << endl;
    return count;
}

void test_codec()
{
    des_key key_bits;
    stringstream in_key("1000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(in_key, key_bits);
    DES_codec des_codec(key_bits);

    stringstream in("0000000000000000000000000000000000000000000000000000000000000000");
    stringstream out;
    des_codec.encrypt_EBC(in, out);

    stringstream out2;
    des_codec.decrypt_EBC(out, out2);

    cout << "message: " << in.str() << endl;
    cout << "desecret:" << out2.str() << endl;
    cout << " secret: " << out.str() << endl << endl;

    count_diff_bits(in.str(), out2.str());
}

void test_rnd_codec()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;
    const size_t N = 10;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_key = u_dis(rnd_engine);
        des_key key_bits(rnd_key);
        DES_codec des_codec(key_bits);

        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        stringstream in(block_bits.to_string());

        stringstream out;
        des_codec.encrypt_EBC(in, out);

        stringstream out2;
        des_codec.decrypt_EBC(out, out2);

        cout << "message: " << in.str() << endl;
        cout << "desecret:" << out2.str() << endl;
        cout << " secret: " << out.str() << endl << endl;

        int diff_bits_count = count_diff_bits(in.str(), out2.str());
        if (diff_bits_count != 0)
        {
            cout << "i: " << i << endl;
            cout << "message: " << in.str() << endl;
            cout << " secret: " << out.str() << endl;
            cout << "desecret:" << out2.str() << endl;
            cout << "diff bits count: " << diff_bits_count << endl << endl;
        }
    }
}

void test_diff_keys()
{
    des_key key1;
    stringstream in_key1("0000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(in_key1, key1);
    DES_codec des_codec1(key1);

    des_key key2;
    stringstream in_key2("1000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(in_key2, key2);
    DES_codec des_codec2(key2);

    count_diff_bits(key1.to_string(), key2.to_string());

    stringstream in1("0000000000000000000000000000000000000000000000000000000000000000");
    stringstream in2("0000000000000000000000000000000000000000000000000000000000000000");

    stringstream out1;
    des_codec1.encrypt_EBC(in1, out1);

    stringstream out2;
    des_codec2.encrypt_EBC(in2, out2);

    cout << "out1: " << out1.str() << endl;
    cout << "out2: " << out2.str() << endl;

    count_diff_bits(out1.str(), out2.str());
}

void test_diff_messages()
{
    des_key key;
    stringstream in_key("0000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(in_key, key);

    DES_codec des_codec1(key);
    DES_codec des_codec2(key);

    stringstream in1("0000000000000000000000000000000000000000000000000000000000000000");
    stringstream in2("1000000000000000000000000000000000000000000000000000000000000000");

    stringstream out1;
    des_codec1.encrypt_EBC(in1, out1);

    stringstream out2;
    des_codec2.encrypt_EBC(in2, out2);

    cout << "out1: " << out1.str() << endl;
    cout << "out2: " << out2.str() << endl;

    count_diff_bits(out1.str(), out2.str());
}

void test_DES_EBC_multi_blocks()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_codec des_codec(key_bits);

    stringstream in;
    const size_t N = 10;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        in << block_bits.to_string();
    }

    stringstream out;
    des_codec.encrypt_EBC(in, out);

    stringstream out2;
    des_codec.decrypt_EBC(out, out2);

    cout << "message: " << in.str() << endl << endl;
    cout << "desecret:" << out2.str() << endl << endl;
    cout << " secret: " << out.str() << endl << endl;

    int diff_bits_count = count_diff_bits(in.str(), out2.str());
    cout << "diff bits count: " << diff_bits_count << endl << endl;

}

void test_DES_CBC_multi_blocks()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_codec des_codec(key_bits);

    stringstream in;
    const size_t N = 10;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        in << block_bits.to_string();
    }

    stringstream out;
    des_codec.encrypt_CBC(in, out);

    stringstream out2;
    des_codec.decrypt_CBC(out, out2);

    cout << "message: " << in.str() << endl << endl;
    cout << "desecret:" << out2.str() << endl << endl;
    cout << " secret: " << out.str() << endl << endl;

    int diff_bits_count = count_diff_bits(in.str(), out2.str());
    cout << "diff bits count: " << diff_bits_count << endl << endl;

}

void compare_DES_EBC_CBC()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_codec des_codec(key_bits);

    stringstream in1;
    stringstream in2;
    const size_t N = 10;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        in1 << block_bits.to_string();
        in2 << block_bits.to_string();
    }

    stringstream out_EBC;
    des_codec.encrypt_EBC(in1, out_EBC);

    stringstream out_CBC;
    des_codec.encrypt_CBC(in2, out_CBC);

    cout << "message: " << in1.str() << endl << endl;
    cout << "EBC_secret:" << out_EBC.str() << endl << endl;
    cout << "CBC_secret: " << out_CBC.str() << endl << endl;

    int diff_bits_count = count_diff_bits(out_EBC.str(), out_CBC.str());
    cout << "diff bits count: " << diff_bits_count << endl << endl;

    stringstream M_EBC;
    des_codec.decrypt_EBC(out_EBC, M_EBC);

    stringstream M_CBC;
    des_codec.decrypt_CBC(out_CBC, M_CBC);

    cout << "EBC_message: " << M_EBC.str() << endl << endl;
    cout << "CBC_message: " << M_CBC.str() << endl << endl;

    int diff_bits_count2 = count_diff_bits(M_EBC.str(), M_CBC.str());
    cout << "diff bits count: " << diff_bits_count2 << endl << endl;
}


int main()
{
    //test_codec();
    //test_rnd_codec();
    //test_diff_keys();
    //test_diff_messages();
//    test_DES_EBC_multi_blocks();
    test_DES_CBC_multi_blocks();
//    compare_DES_EBC_CBC();
    return 0;
}

