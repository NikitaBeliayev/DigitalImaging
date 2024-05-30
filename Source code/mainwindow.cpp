#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QLabel>
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const QSize btn_size = QSize(130, 70);
    open_image_btn = new QPushButton("open image");
    open_image_btn->setFixedSize(btn_size);
    vertical_layout = new QVBoxLayout();
    horizontal_layout = new QHBoxLayout();
    histogram_algo_btn = new QPushButton("histo algo");
    histogram_algo_btn->setFixedSize(btn_size);
    filter_algo_btn = new QPushButton("filter");
    filter_algo_btn->setFixedSize(btn_size);
    image_label = new QLabel();
    vertical_layout->addWidget(filter_algo_btn);
    vertical_layout->addWidget(histogram_algo_btn);
    vertical_layout->addWidget(open_image_btn);
    horizontal_layout->addLayout(vertical_layout);
    horizontal_layout->addWidget(image_label);
    QWidget* widget = new QWidget();
    widget->setLayout(horizontal_layout);
    this->setCentralWidget(widget);
    connect(this->open_image_btn, SIGNAL(clicked()), SLOT(OpenFileDialog()));
    connect(this->histogram_algo_btn, SIGNAL(clicked()), SLOT(HistogramAlgorithm()));
    connect(this->filter_algo_btn, SIGNAL(clicked()), SLOT(FilterAlgorithm()));
    this->setFixedHeight(900);
    this->setFixedWidth(900);
}

void MainWindow::HistogramAlgorithm(){

    cv::Mat src = cv::imread(path.toUtf8().data());
    if(!src.empty())
    {
        std::vector<cv::Mat> bgr_planes;
        split( src, bgr_planes );
        int histSize = 256;
        float range[] = { 0, 256 }; //the upper boundary is exclusive
        const float* histRange[] = { range };
        bool uniform = true, accumulate = false;
        cv::Mat b_hist, g_hist, r_hist;
        calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
        calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
        calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
        int hist_w = 512, hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );
        cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );
        normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
        for( int i = 1; i < histSize; i++ )
        {
            line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
                  cv::Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                  cv::Scalar( 255, 0, 0), 2, 8, 0  );
            line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
                  cv::Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                  cv::Scalar( 0, 255, 0), 2, 8, 0  );
            line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
                  cv::Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                  cv::Scalar( 0, 0, 255), 2, 8, 0  );
        }
        imshow("calcHist Demo", histImage );
    }
}

void MainWindow::FilterAlgorithm(){
        if (path != ""){
            cv::Mat src = cv::imread(path.toUtf8().data(), 1);
            cv::Mat dst;         
            GaussianBlur( src, dst, cv::Size( 7, 7 ), 0, 0 );
            cv::Mat result;
            cv::resize(dst, result, cv::Size(1024, 768), 0, 0, cv::INTER_CUBIC);
            imshow("Low Pass Filtered Image", result);
        }

}

void MainWindow::OpenFileDialog(){
    path = QFileDialog::getOpenFileName(this, "choose image", "/", "All Files(*.*);; JPEG Image(*.jpg);; PNG IMage(*.png);");
    QImage image(path);
    image_label->setPixmap(QPixmap::fromImage(image.scaled(700, 700)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

