#include "sha256.h"

#include <crypto++/modes.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/sha.h>
#include <crypto++/hex.h>

using namespace std;

SHA256::SHA256()
{
}

vector<char> SHA256::computeHash(vector<char> message) {

    CryptoPP::SHA256 hash;
    byte digest[ CryptoPP::SHA256::DIGESTSIZE ];

    char pb[message.size()];
    for(int i =0; i<message.size(); i++) {
        pb[i] = message[i];
    }

    hash.CalculateDigest( digest, (byte*)pb, message.size());

    vector<char> result;
    for(int i = 0; i<CryptoPP::SHA256::DIGESTSIZE;i++) {
        result.push_back(digest[i]);
    }

    return result;  
}
