#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include <QApplication>

#include "ThreadSafeQueue.hpp"

#include "Payload.hpp"
#include "ClientWindow.hpp"
#include "ServerWidget.hpp"

int main(int argc, char *argv[]) {
    qRegisterMetaType<Payload>("Payload");
    QApplication app(argc, argv);
    ThreadSafeQueue<Payload> queue;

    ClientWindow clientWindow(queue);
    clientWindow.show();

    ServerWidget serverWidget(clientWindow.window(), queue);
    serverWidget.show();

    return app.exec();
}