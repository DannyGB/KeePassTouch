#include "salsa20.h"

#include "./cryptopp/modes.h"
#include "./cryptopp/salsa.h"
#include "./cryptopp/osrng.h"

using namespace std;

vector<char> m_xpbKey;
char* m_IV;
CryptoPP::Salsa20::Encryption salsa;

Salsa20::Salsa20(vector<char> pbKey, char* pbIv)
{
    m_xpbKey = pbKey;
    m_IV = pbIv;
    byte key[32], iv[8];

    for(int i=0;i<32;i++) {
        key[i] = m_xpbKey[i];
    }

    for(int i=0;i<8;i++) {
        iv[i] = m_IV[i];
    }

    salsa.SetKeyWithIV(key, 32, iv);
}

byte* Salsa20::decrypt(vector<char> cipherText)
{
    byte* cipherTextBytes = new byte[cipherText.size()];
    byte* plainTextBytes = new byte[cipherText.size()];
    string retVal("");    

    for(int i=0;i<cipherText.size();i++) {
        cipherTextBytes[i] = cipherText[i];
    }

    salsa.ProcessData(plainTextBytes, cipherTextBytes, cipherText.size());

    //delete cipherTextBytes;
    return plainTextBytes;
}

