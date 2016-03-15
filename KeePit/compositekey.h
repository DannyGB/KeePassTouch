#ifndef COMPOSITEKEY_H
#define COMPOSITEKEY_H

#include <QObject>
#include "./cryptopp/modes.h"

using namespace std;

class CompositeKey
{
public:
    CompositeKey(vector<char>, vector<char>);
    vector<char> generateKey32(vector<char>, ulong);

protected:
    vector<char> createRawCompositeKey(vector<char>);
    vector<char> transformKey(vector<char>, vector<char>, ulong);

private:
    vector<char> m_pbKey;
    vector<char> m_pbKeyFile;
    bool hasKeyFile = false;

Q_SIGNALS:

public slots:
};

#endif // COMPOSITEKEY_H
