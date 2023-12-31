#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <loguru/loguru.hpp>
#include "db_notice_operator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    loguru::init(argc,argv);

    QQmlApplicationEngine engine;
    LOG_F(INFO,"QQmlApplicationEngine start");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("EquisetumReview", "Main");
    LOG_F(INFO, "Load the main qml module");

    return QGuiApplication::exec();
}
