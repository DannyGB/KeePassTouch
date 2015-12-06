//#include <QtQml>
//#include <QtQml/QQmlContext>
#include "backend.h"
#include "filesystem.h"
//#include "passwordentrymodel.h"
#include "passwordentry.h"


void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("KeePass"));

    //qmlRegisterType<Filesystem>(uri, 1, 0, "Filesystem");
    //qmlRegisterType<PasswordEntryModel>(uri, 1, 0, "PasswordEntryModel");
    //qmlRegisterType<PasswordEntry>(uri, 1, 0, "PasswordEntry");
}

void BackendPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}

