#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <qqmlcontext.h>

#include "filesystem.h"
#include "passwordentry.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Filesystem fs;
    PasswordEntryModel* model = fs.createModel();

    qmlRegisterType<Filesystem>("KeePass3", 1, 0, "Filesystem");

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("fileSystem", &fs);
    ctxt->setContextProperty("passwordEntryModel", model);
    view.show();
    return app.exec();
}

