#ifndef AES_H
#define AES_H

#include <string>
#include "/home/dan/KeePass3/KeePass3/cryptopp/modes.h"

class Aes
{
public:
    Aes();
    std::string decrypt(byte * pbAesKey, uint keysize, byte* pbEncryptionIV, byte* pbFileContent, uint contentSize);
};

#endif // AES_H
