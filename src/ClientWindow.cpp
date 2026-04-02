#include <iostream>

#include <QWidget>
#include <QLabel>
#include <QMainWindow>
#include <QFormLayout>
#include <QLineEdit>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QPushButton>

#include "ThreadSafeQueue.hpp"

#include "Payload.hpp"
#include "ClientWindow.hpp"

ClientWindow::ClientWindow(IWriteOnlyQueue<Payload>& writer): QMainWindow(), writer_(writer) {
    this->setWindowTitle("Client Window");
    this->resize(500, 400);
    this->move(100, 200);

    QWidget *centralWidget = new QWidget(this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignLeft);

    QLabel *nameLabel = new QLabel("Enter your name:");
    nameInput_ = new QLineEdit();
    formLayout->addRow(nameLabel, nameInput_);

    QLabel *dateLabel = new QLabel("Select Date:");
    dateEdit_ = new QDateEdit(QDate::currentDate());
    dateEdit_->setCalendarPopup(true);
    formLayout->addRow(dateLabel, dateEdit_);

    QLabel *timeLabel = new QLabel("Select Time:");
    timeEdit_ = new QTimeEdit(QTime::currentTime());
    timeEdit_->setDisplayFormat("HH:mm");
    formLayout->addRow(timeLabel, timeEdit_);

    QPushButton *submitButton = new QPushButton("Submit");
    formLayout->addRow(submitButton);
    QObject::connect(submitButton, &QPushButton::clicked, [this]() {
        this->submit();
    });

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch(); 

    this->setCentralWidget(centralWidget);
}

void ClientWindow::submit() {
    QDate date = dateEdit_->date();
    QTime time = timeEdit_->time();

    Payload payload{
        nameInput_->text().toStdString(),
        {
            static_cast<int>(date.year()),
            static_cast<int>(date.month()),
            static_cast<int>(date.day())
        },
        {
            time.hour(),
            time.minute(),
        }
    };
    
    writer_.push(payload);
    std::cout << "[Client Window] Payload sent to queue." << std::endl;
}

void ClientWindow::closeEvent(QCloseEvent *event) {
    std::cout << "[Client Window] Closing communication queue..." << std::endl;
    writer_.close();
    std::cout << "[Client Window] Communication queue closed successfully." << std::endl;
    event->accept();
}
