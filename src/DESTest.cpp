
#include "DESTest.h"

#include <random>
#include <ctime>

using std::default_random_engine;
using std::uniform_int_distribution;

size_t count_diff_bits(const string &bitstring1, const string &bitstring2)
{
    size_t diff_bits_count = 0;
    for (size_t i = 0; i < bitstring1.size() && i < bitstring2.size(); i++)
        if (bitstring1[i] != bitstring2[i])
            diff_bits_count++;
    return diff_bits_count;
}

size_t count_diff_bits(const string &bitstring1, const string &bitstring2, bool print_count_flag)
{
    int diff_bits_count = count_diff_bits(bitstring1, bitstring2);
    if (print_count_flag)
        cout << "different bits count: " << diff_bits_count << endl;
    return diff_bits_count;
}

void test_codec()
{
    des_key key_bits;
    stringstream key_stream("1000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(key_stream, key_bits);
    DES_Codec des_codec(key_bits);

    stringstream message("0000000000000000000000000000000000000000000000000000000000000000");
    stringstream secret;
    des_codec.encrypt_EBC(message, secret);

    stringstream decode_secret;
    des_codec.decrypt_EBC(secret, decode_secret);

    cout << "          key: " << key_bits.to_string() << endl;
    cout << "      message: " << message.str() << endl;
    cout << "       secret: " << secret.str() << endl;
    cout << "decode_secret: " << decode_secret.str() << endl << endl;

    count_diff_bits(message.str(), decode_secret.str(), true);
}

void test_rnd_codec()
{
    default_random_engine rnd_engine(time(0));
    uniform_int_distribution<unsigned long> u_dis;
    const size_t N = 1000;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_key = u_dis(rnd_engine);
        des_key key_bits(rnd_key);
        DES_Codec des_codec(key_bits);

        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        stringstream message(block_bits.to_string());

        stringstream secret;
        des_codec.encrypt_EBC(message, secret);

        stringstream decode_secret;
        des_codec.decrypt_EBC(secret, decode_secret);

        cout << "          key: " << key_bits.to_string() << endl;
        cout << "      message: " << message.str() << endl;
        cout << "       secret: " << secret.str() << endl;
        cout << "decode_secret: " << decode_secret.str() << endl << endl;

        int diff_bits_count = count_diff_bits(message.str(), decode_secret.str());
        if (diff_bits_count != 0)
        {
            cout << "Decode secret isn't equal to the original message!!!!!" << endl;
            cout << "          key: " << key_bits.to_string() << endl;
            cout << "      message: " << message.str() << endl;
            cout << "       secret: " << secret.str() << endl;
            cout << "decode_secret: " << decode_secret.str() << endl << endl;
            exit(1);
        }
    }
    cout << "All decode secrets are the same as the original message!" << endl;
}

void test_diff_keys()
{
    des_key key1;
    stringstream key1_stream("1110001011110110110111100011000000111010000010000110001011011100");
    IOAdapter::read_bits(key1_stream, key1);
    DES_Codec des_codec1(key1);

    des_key key2;
    stringstream key2_stream("0110001011110110110111100011000000111010000010000110001011011100");
    IOAdapter::read_bits(key2_stream, key2);
    DES_Codec des_codec2(key2);

    stringstream message1("0110100010000101001011110111101000010011011101101110101110100100");
    stringstream message2(message1.str());

    stringstream secret1;
    des_codec1.encrypt_EBC(message1, secret1);

    stringstream secret2;
    des_codec2.encrypt_EBC(message2, secret2);

    // 别直接输出 key1 或 key2，little-endian 会颠倒顺序的
//    cout << "    key1: " << key1.to_string() << endl;
//    cout << "    key2: " << key2.to_string() << endl;
//    count_diff_bits(key1.to_string(), key2.to_string(), true);

    cout << "    key1: " << key1_stream.str() << endl;
    cout << "    key2: " << key2_stream.str() << endl;
    count_diff_bits(key1_stream.str(), key2_stream.str(), true);
    cout << endl;

    cout << "message1: " << message1.str() << endl;
    cout << "message2: " << message2.str() << endl;
    count_diff_bits(message1.str(), message2.str(), true);
    cout << endl;

    cout << " secret1: " << secret1.str() << endl;
    cout << " secret2: " << secret2.str() << endl;
    count_diff_bits(secret1.str(), secret2.str(), true);
}

void test_diff_messages()
{
    des_key key;
    stringstream key_stream("0000001010010110010010001100010000111000001100000011100001100100");
    IOAdapter::read_bits(key_stream, key);

    DES_Codec des_codec1(key);
    DES_Codec des_codec2(key);

    stringstream message1("0000000000000000000000000000000000000000000000000000000000000000");
    stringstream message2("1000000000000000000000000000000000000000000000000000000000000000");

    stringstream secret1;
    des_codec1.encrypt_EBC(message1, secret1);

    stringstream secret2;
    des_codec2.encrypt_EBC(message2, secret2);

    cout << "     key: " << key_stream.str() << endl;
    cout << endl;

    cout << "message1: " << message1.str() << endl;
    cout << "message2: " << message2.str() << endl;
    count_diff_bits(message1.str(), message2.str(), true);
    cout << endl;

    cout << " secret1: " << secret1.str() << endl;
    cout << " secret2: " << secret2.str() << endl;

    count_diff_bits(secret1.str(), secret2.str(), true);
}

void test_DES_multi_blocks_EBC()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_Codec des_codec(key_bits);

    stringstream message;
    const size_t N = 5;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        message << block_bits.to_string();
    }

    stringstream secret;
    des_codec.encrypt_EBC(message, secret);

    stringstream decode_secret;
    des_codec.decrypt_EBC(secret, decode_secret);


    cout << "key: ";
    IOAdapter::write_bits(cout, key_bits);
    cout << endl;

    cout << "message: \n" << message.str() << endl << endl;
    cout << "secret: \n" << secret.str() << endl << endl;
    cout << "decode_secret: \n" << decode_secret.str() << endl << endl;

    count_diff_bits(message.str(), decode_secret.str(), true);
}

void test_DES_multi_blocks_CBC()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_Codec des_codec(key_bits);

    stringstream message;
    const size_t N = 5;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        message << block_bits.to_string();
    }

    stringstream secret;
    des_codec.encrypt_CBC(message, secret);

    stringstream decode_secret;
    des_codec.decrypt_CBC(secret, decode_secret);

    cout << "key: ";
    IOAdapter::write_bits(cout, key_bits);
    cout << endl;

    cout << "message: \n" << message.str() << endl << endl;
    cout << "secret: \n" << secret.str() << endl << endl;
    cout << "decode_secret: \n" << decode_secret.str() << endl << endl;

    count_diff_bits(message.str(), decode_secret.str(), true);
}

void cmp_DES_EBC_CBC()
{
    default_random_engine rnd_engine;
    uniform_int_distribution<unsigned long> u_dis;

    unsigned long rnd_key = u_dis(rnd_engine);
    des_key key_bits(rnd_key);
    DES_Codec des_codec(key_bits);

    stringstream message1;
    const size_t N = 5;
    for (size_t i = 0; i < N; i++)
    {
        unsigned long rnd_block = u_dis(rnd_engine);
        des_block block_bits(rnd_block);
        message1 << block_bits.to_string();
    }
    stringstream message2(message1.str());

    stringstream EBC_secret;
    des_codec.encrypt_EBC(message1, EBC_secret);

    stringstream CBC_secret;
    des_codec.encrypt_CBC(message2, CBC_secret);

    cout << "key: ";
    IOAdapter::write_bits(cout, key_bits);
    cout << endl << endl;
    cout << "message: \n" << message1.str() << endl << endl;
    cout << "EBC_secret:\n" << EBC_secret.str() << endl << endl;
    cout << "CBC_secret:\n" << CBC_secret.str() << endl << endl;

    count_diff_bits(EBC_secret.str(), CBC_secret.str(), true);
    cout << endl;

    stringstream EBC_decode_secret;
    des_codec.decrypt_EBC(EBC_secret, EBC_decode_secret);

    stringstream CBC_decode_secret;
    des_codec.decrypt_CBC(CBC_secret, CBC_decode_secret);

    cout << "EBC_decode_secret: \n" << EBC_decode_secret.str() << endl << endl;
    cout << "CBC_decode_secret: \n" << CBC_decode_secret.str() << endl << endl;
    count_diff_bits(EBC_decode_secret.str(), CBC_decode_secret.str(), true);
    cout << endl;
}

void cmp_diff_keys_EBC_CBC()
{
    const size_t BLOCK_COUNT = 5;
    const size_t MAX_EPOCH = 1000;

    default_random_engine rnd_engine(time(NULL));
    uniform_int_distribution<unsigned long> u_dis;
    uniform_int_distribution<unsigned int> rnd_key_bit_idx_dis(0, BLOCK_SIZE - 1);

    double EBC_diff_bits_count = 0;
    double CBC_diff_bits_count = 0;

    for (size_t epoch = 0; epoch < MAX_EPOCH; epoch++)
    {
        stringstream message;
        for (size_t i = 0; i < BLOCK_COUNT; i++)
        {
            unsigned long rnd_block = u_dis(rnd_engine);
            des_block block_bits(rnd_block);
            message << block_bits.to_string();
        }

        unsigned long rnd_key = u_dis(rnd_engine);
        des_key key1_bits(rnd_key);
        DES_Codec des_codec1(key1_bits);

        size_t rnd_flip_bit_idx = rnd_key_bit_idx_dis(rnd_engine);
        des_key key2_bits = key1_bits;
        key2_bits.flip(rnd_flip_bit_idx);
        DES_Codec des_codec2(key2_bits);

        if (count_diff_bits(key1_bits.to_string(), key2_bits.to_string()) != 1){
            cerr << "error: key1 and key2 don't has 1 different bit" << endl;
            cerr << "key1: " << key1_bits.to_string() << endl;
            cerr << "key2: " << key2_bits.to_string() << endl << endl;
            exit(1);
        }

        stringstream EBC_secret1;
        stringstream message1(message.str());
        des_codec1.encrypt_EBC(message1, EBC_secret1);

        stringstream EBC_secret2;
        stringstream message2(message.str());
        des_codec2.encrypt_EBC(message2, EBC_secret2);

        EBC_diff_bits_count += count_diff_bits(EBC_secret1.str(), EBC_secret2.str());

        stringstream CBC_secret1;
        stringstream message3(message.str());
        des_codec1.encrypt_CBC(message3, CBC_secret1);

        stringstream CBC_secret2;
        stringstream message4(message.str());
        des_codec2.encrypt_CBC(message4, CBC_secret2);

        CBC_diff_bits_count += count_diff_bits(CBC_secret1.str(), CBC_secret2.str());
    }

    double avg_EBC_diff_bits_count = EBC_diff_bits_count / MAX_EPOCH;
    double avg_CBC_diff_bits_count = CBC_diff_bits_count / MAX_EPOCH;

    double avg_EBC_diff_bits_percentage = avg_EBC_diff_bits_count / (BLOCK_SIZE * BLOCK_COUNT);
    double avg_CBC_diff_bits_percentage = avg_CBC_diff_bits_count / (BLOCK_SIZE * BLOCK_COUNT);

    printf("%d blocks, %d epoches\n", BLOCK_COUNT, MAX_EPOCH);

    printf("average EBC_diff_bits_count: %.2f, %.2f%%\n",
            avg_EBC_diff_bits_count, avg_EBC_diff_bits_percentage);
    printf("average CBC_diff_bits_count: %.2f, %.2f%%\n",
            avg_CBC_diff_bits_count, avg_CBC_diff_bits_percentage);
}

