#pragma once

#include <thread>

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QCloseEvent>

#include "ThreadSafeQueue.hpp"
#include "Payload.hpp"

class ServerWidget: public QMainWindow{
    Q_OBJECT

signals:
    void payloadReceived(const Payload& payload);

public slots:
    void displayPayload(const Payload& payload);

private:
    QVBoxLayout *layout_;
    IReadOnlyQueue<Payload>& reader_;
    std::thread consumer_;

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    ServerWidget(QMainWindow* mainWindow, IReadOnlyQueue<Payload>& reader);
};