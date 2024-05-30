#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include<QLabel>
#include <opencv2/objdetect.hpp>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT



private slots:
    void OpenFileDialog();
    void HistogramAlgorithm();
    void FilterAlgorithm();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString path = "";
    Ui::MainWindow *ui;
    QPushButton* histogram_algo_btn;
    QPushButton* filter_algo_btn;
    QLabel* image_label;
    QVBoxLayout* vertical_layout;
    QHBoxLayout* horizontal_layout;
    QPushButton* open_image_btn;

};
#endif // MAINWINDOW_H
