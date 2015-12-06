//#include <pthread.h>
#include <cryptopp/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/cryptlib.h>
#include <crypto++/rijndael.h>

#include "compositekey.h"
#include "sha256.h"

/*#include "/home/dan/KeePass/libs/modes.h"
#include "/home/dan/KeePass/libs/aes.h"
#include "/home/dan/KeePass/libs/filters.h"
#include </home/dan/KeePass/libs/cryptlib.h>
#include </home/dan/KeePass/libs/rijndael.h>*/

#define AES256_KEY_LENGTH 32
#define ROUND_UP_TO_MULTIPLE(numberToRound, multiple) \
    ((numberToRound + multiple - 1) & ~(multiple - 1))

using namespace std;
using namespace CryptoPP;

#define MASTER_KEY_SIZE 32

struct String {
    char *value;
    size_t length;
};

vector<char> m_pbKey;

CompositeKey::CompositeKey(vector<char> pbKey)
{
    m_pbKey = pbKey;
}

vector<char> CompositeKey::generateKey32(vector<char> pbKeySeed32, ulong uNumRounds) {
    assert(pbKeySeed32.size() == MASTER_KEY_SIZE);
    if(pbKeySeed32.size() != MASTER_KEY_SIZE) {
        throw std::exception();
    }

    vector<char> pbRaw32 = createRawCompositeKey(m_pbKey);
    if(pbRaw32.size() != MASTER_KEY_SIZE) {
        throw std::exception();
    }

    // Hash the hash, this is due to the fact that KeePass hashes each part of the comp key then
    // concatenates those hashes, then hashes the composite together.
    pbRaw32 = createRawCompositeKey(pbRaw32);
    if(pbRaw32.size() != MASTER_KEY_SIZE) {
        throw std::exception();
    }

    vector<char> pbTrf32 = transformKey(pbRaw32, pbKeySeed32, uNumRounds);
    if(pbTrf32.size() != MASTER_KEY_SIZE) {
        throw std::exception();
    }

    return pbTrf32;
}

vector<char> CompositeKey::createRawCompositeKey(vector<char> key) {

    SHA256 sha256;
    vector<char> hash = sha256.computeHash(key);

    return hash;
}

vector<char> CompositeKey::transformKey(vector<char> pbOriginalKey32, vector<char> pbKeySeed32, ulong uNumRounds) {
    if(pbOriginalKey32.size() != MASTER_KEY_SIZE) throw new std::exception();
    assert((pbKeySeed32.size() == MASTER_KEY_SIZE));
    if(pbKeySeed32.size() != MASTER_KEY_SIZE) throw new std::exception();

    struct String untransformedKey;
    struct String unpaddedSeed;

    untransformedKey.length = pbOriginalKey32.size();
    untransformedKey.value = &pbOriginalKey32[0];
    unpaddedSeed.length = pbKeySeed32.size();
    unpaddedSeed.value = &pbKeySeed32[0];

    struct String paddedSeed;
    paddedSeed.length = AES256_KEY_LENGTH;
    paddedSeed.value = (char *) malloc(AES256_KEY_LENGTH);
    memset(paddedSeed.value, 0, paddedSeed.length);
    memcpy(paddedSeed.value, unpaddedSeed.value, paddedSeed.length);

    CryptoPP::AES::Encryption cppAesEncryption((const byte *) paddedSeed.value, AES256_KEY_LENGTH);
    CryptoPP::ECB_Mode_ExternalCipher::Encryption cppEbcAesEncryptor(cppAesEncryption);

    struct String transformedKey;
    transformedKey.length = ROUND_UP_TO_MULTIPLE(untransformedKey.length, CryptoPP::AES::BLOCKSIZE);
    transformedKey.value = (char *) malloc(transformedKey.length);
    memset(transformedKey.value, 0, transformedKey.length);
    memcpy(transformedKey.value, untransformedKey.value, transformedKey.length);

    int aesBlockCount = transformedKey.length / CryptoPP::AES::BLOCKSIZE;

    for(; uNumRounds > 0; uNumRounds--) {
        for(int block = 0; block < aesBlockCount; block++) {
            size_t currentOffset = block * CryptoPP::AES::BLOCKSIZE;
            cppEbcAesEncryptor.ProcessData(
                (byte *) (transformedKey.value + currentOffset),
                (const byte *) (transformedKey.value + currentOffset),
                CryptoPP::AES::BLOCKSIZE
            );
        }
    }

    vector<char> result;

    for(int i = 0; i<MASTER_KEY_SIZE;i++) {
        result.push_back(transformedKey.value[i]);
    }


    SHA256 sha256;
    return sha256.computeHash(result);
}
