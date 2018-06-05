#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "datacapture.h"
#include <QMainWindow>
#include <QFileDialog>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void timerEvent(QTimerEvent *event);
    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_replay_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
