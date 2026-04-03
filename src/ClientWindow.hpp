#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QCloseEvent>

#include "ThreadSafeQueue.hpp"
#include "ServerWindow.hpp"
#include "Payload.hpp"

class ClientWindow: public QMainWindow {
    Q_OBJECT

private:
    int id_;
    ServerWindow *parent_;
    IWriteOnlyQueue<Payload>& writer_;

    QLineEdit *nameInput_;
    QDateEdit *dateEdit_;
    QTimeEdit *timeEdit_;

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    ClientWindow(ServerWindow* mainWindow, IWriteOnlyQueue<Payload>& writer);
    void submit();
};