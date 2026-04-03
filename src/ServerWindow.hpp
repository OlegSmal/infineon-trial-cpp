#pragma once

#include <thread>
#include <atomic>
#include <unordered_map>

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QCloseEvent>

#include "ThreadSafeQueue.hpp"
#include "Payload.hpp"

class ServerWindow: public QMainWindow {
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
    std::atomic<int> liveClients_;
    std::unordered_map<int, QMainWindow*> clients_;
    friend class ClientWindow;

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    ServerWindow(IReadOnlyQueue<Payload>& reader);
};