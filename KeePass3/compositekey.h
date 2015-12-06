#ifndef COMPOSITEKEY_H
#define COMPOSITEKEY_H

#include <QObject>
#include <crypto++/modes.h>

using namespace std;

class CompositeKey
{
public:
    CompositeKey(vector<char>);
    vector<char> generateKey32(vector<char>, ulong);

protected:
    vector<char> createRawCompositeKey(vector<char>);
    vector<char> transformKey(vector<char>, vector<char>, ulong);

Q_SIGNALS:

public slots:
};

#endif // COMPOSITEKEY_H
