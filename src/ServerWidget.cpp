#include <iostream>
#include <optional>
#include <string>
#include <thread>

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

#include "Payload.hpp"
#include "ServerWidget.hpp"

ServerWidget::ServerWidget(QMainWindow* mainWindow, IReadOnlyQueue<Payload>& reader): QMainWindow(mainWindow), reader_(reader) {
    connect(this, &ServerWidget::payloadReceived, this, &ServerWidget::displayPayload);


    this->setWindowTitle("Server Window");
    this->resize(500, 400);
    this->move(650, 200);
    this->setWindowFlags(Qt::Window);
    
    // disable the close button so that this widget is shut down gracefully after main window is closed
    Qt::WindowFlags flags = this->windowFlags();
    flags |= Qt::CustomizeWindowHint; 
    flags |= Qt::WindowTitleHint;
    flags &= ~Qt::WindowCloseButtonHint;
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowMaximizeButtonHint;
    this->setWindowFlags(flags);

    QWidget *centralWidget = new QWidget(this);
    layout_ = new QVBoxLayout(centralWidget);
    layout_->addStretch(1); 

    this->setCentralWidget(centralWidget);

    consumer_ = std::thread([this]() {
        std::cout << "[Server Window] Starting queue consumer..." << std::endl;
        
        while (true) {
            std::optional<Payload> payload = reader_.pop();
            if (!payload) { 
                std::cout << "[Server Window] Received queue close. Exiting..." << std::endl;
                break; 
            }
            emit payloadReceived(payload.value());
            std::cout << "[Server Window] Payload received." << std::endl;
        }
    });
}

void ServerWidget::displayPayload(const Payload& payload) {
    int index = layout_->count() - 1;

    QLabel *nameLabel = new QLabel(
        QString("<b>Name:</b> %1")
        .arg(payload.clientName)
    );
    layout_->insertWidget(index++, nameLabel);
    nameLabel->show();

    QLabel *dateLabel = new QLabel(
        QString("<b>Date:</b> %1-%2-%3")
        .arg(payload.date.year)
        .arg(payload.date.month)
        .arg(payload.date.day)
    );
    layout_->insertWidget(index++, dateLabel);
    dateLabel->show();

    QLabel *timeLabel = new QLabel(
        QString("<b>Time:</b> %1:%2")
        .arg(payload.time.hour)
        .arg(payload.time.minute)
    );
    layout_->insertWidget(index++, timeLabel);
    timeLabel->show();

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("color: #fff;");
    layout_->insertWidget(index++, line);
    line->show();
}

void ServerWidget::closeEvent(QCloseEvent *event) {
    std::cout << "[Server Window] Joining consumer thread..." << std::endl;
    consumer_.join(); 
    std::cout << "[Server Window] Consumer thread gracefully shut down." << std::endl;
    event->accept();
}
