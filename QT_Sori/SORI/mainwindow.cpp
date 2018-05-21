#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList interfaceList = DataCapture::getInterfacesList();
    for (const auto& str : interfaceList) {
        ui->listWidget->addItem(str);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
