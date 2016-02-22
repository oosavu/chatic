#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include "chatmanager.h"
#include "tinyaes.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ChatManager chatManager;

    QQmlApplicationEngine engine;
    qmlRegisterType<User>("CppImport",1,0,"user");
    engine.rootContext()->setContextProperty("chatManager",&chatManager);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
