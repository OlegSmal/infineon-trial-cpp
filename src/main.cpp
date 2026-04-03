#include <QApplication>

#include "ThreadSafeQueue.hpp"

#include "Payload.hpp"
#include "ClientWindow.hpp"
#include "ServerWindow.hpp"

int main(int argc, char *argv[]) {
    qRegisterMetaType<Payload>("Payload");
    QApplication app(argc, argv);
    ThreadSafeQueue<Payload> queue;

    ServerWindow serverWindow(queue);
    serverWindow.show();

    ClientWindow clientWindow1(&serverWindow, queue);
    clientWindow1.show();

    ClientWindow clientWindow2(&serverWindow, queue);
    clientWindow2.show();

    return app.exec();
}