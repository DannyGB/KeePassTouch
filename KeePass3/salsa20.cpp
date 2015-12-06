#include "salsa20.h"

using namespace std;

vector<char> m_xpbKey;
vector<char> m_IV;

Salsa20::Salsa20(vector<char> pbKey, vector<char> pbIv)
{
    m_xpbKey = pbKey;
    m_IV = pbIv;
}

