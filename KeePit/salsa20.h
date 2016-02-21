#ifndef SALSA20_H
#define SALSA20_H

#include <QObject>
#include "./cryptopp/modes.h"

using namespace std;

class Salsa20
{
public:
    Salsa20(vector<char> key, char* iv);
    byte* decrypt(vector<char>);

Q_SIGNALS:

public slots:
};

#endif // SALSA20_H
