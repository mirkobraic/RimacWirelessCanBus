#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CanBusManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qmlRegisterUncreatableType<CanBusManager>("com.RimacWirelessCanBus", 1, 0, "ConnectionStatus", "Cannot create enum in qml");

    CanBusManager manager(kvaser);
    engine.rootContext()->setContextProperty(QStringLiteral("canBusManager"), &manager);
    engine.load(url);

    return app.exec();
}
