#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <qqmlcontext.h>

#include "filesystem.h"
#include "passwordentry.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    PasswordEntryModel model;
    model.addPasswordEntry(PasswordEntry("Sample", "1234"));
    model.addPasswordEntry(PasswordEntry("elpmaS", "4321"));

    qmlRegisterType<Filesystem>("KeePass3", 1, 0, "Filesystem");
    //qmlRegisterType<PasswordEntry>("KeePass3", 1, 0, "PasswordEntry");

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qrc:///Main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    QQmlContext *ctxt = view.rootContext();
    ctxt->setContextProperty("passwordEntryModel", &model);
    view.show();
    return app.exec();
}

