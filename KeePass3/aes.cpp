#include "aes.h"
#include <string>

#include "/home/dan/KeePass3/KeePass3/cryptopp/modes.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/aes.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/filters.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/sha.h"
#include "/home/dan/KeePass3/KeePass3/cryptopp/osrng.h"

using namespace std;

Aes::Aes()
{
}

string Aes::decrypt(byte * pbAesKey, uint keysize, byte* pbEncryptionIV, byte* pbFileContent, uint contentSize) {

    string recovered;
    CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption d;
    d.SetKeyWithIV(pbAesKey, keysize, pbEncryptionIV );

    // The StreamTransformationFilter removes
    //  padding as required.
    CryptoPP::ArraySource ss(pbFileContent, contentSize, true,
        new CryptoPP::StreamTransformationFilter( d,
            new CryptoPP::StringSink( recovered )
        ) // StreamTransformationFilter
    ); // StringSource

    return recovered;
}
