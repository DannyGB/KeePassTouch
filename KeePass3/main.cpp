#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <qqmlcontext.h>

#include "database.h"
#include "passwordentry.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Database fs;
    PasswordEntryModel* model = fs.createModel();

    qmlRegisterType<Database>("KeePass3", 1, 0, "Database");

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("database", &fs);
    ctxt->setContextProperty("passwordEntryModel", model);
    view.show();
    return app.exec();
}
