#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cmdImageSnap_clicked();
    void Record_Video();

    void on_cmdRecord_clicked();

    void on_cmdStopRecord_clicked();

    void on_hslideLH_sliderMoved(int position);

    void on_hslideLS_sliderMoved(int position);

    void on_hslideLV_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
