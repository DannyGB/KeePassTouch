#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
//typedef unsigned char BYTE;

class Base64 {

public:
    Base64();
    std::string base64_encode(char const* buf, unsigned int bufLen);
    std::vector<char> base64_decode(std::string const&);
};
#endif
