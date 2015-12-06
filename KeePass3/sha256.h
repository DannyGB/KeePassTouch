#ifndef SHA256_H
#define SHA256_H

#include <QObject>

using namespace std;

class SHA256
{
public:
    SHA256();
    vector<char> computeHash(vector<char>);

Q_SIGNALS:

public slots:
};

#endif // SHA256_H
