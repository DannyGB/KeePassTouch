#include "aes.h"
#include <string>

#include "./cryptopp/modes.h"
#include "./cryptopp/aes.h"
#include "./cryptopp/filters.h"
#include "./cryptopp/sha.h"
#include "./cryptopp/osrng.h"

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
