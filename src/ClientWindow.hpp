#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QCloseEvent>

#include "ThreadSafeQueue.hpp"
#include "Payload.hpp"

class ClientWindow: public QMainWindow {
    Q_OBJECT

private:
    IWriteOnlyQueue<Payload>& writer_;

    QLineEdit *nameInput_;
    QDateEdit *dateEdit_;
    QTimeEdit *timeEdit_;

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    ClientWindow(IWriteOnlyQueue<Payload>& writer);
    void submit();
};