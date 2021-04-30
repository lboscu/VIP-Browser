#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QWebEngineSettings>
#include<QWebChannel>
#include<QUrl>
#include<QDebug>
#include<QProgressBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    progress = new QProgressBar;
    this->showMaximized();
    mytabwidget = ui->tabWidget;
    edit = ui->lineEdit;
    stabar = ui->statusbar;

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    ui->lineEdit->setText("http://www.baidu.com/");

    //---------------------------------------------------------------------------------------
    MyWebView* webview = new MyWebView;
    qlist.append(webview);
    webview->load(QUrl("http://www.baidu.com/"));

    ui->tabWidget->addTab(webview,"tab");

    connect(webview,&MyWebView::titleChanged,this,[=](){
            ui->tabWidget->setTabText(0,webview->title());
    });
    connect(webview,&MyWebView::iconChanged,this,[=](QIcon ic){
        mytabwidget->setTabIcon(0,ic);
    });

    //---------------------------------------------------------------------------------------
    connect(mytabwidget,&MyTabWidget::tabBarDoubleClicked,this,[=](int index){
        emit mytabwidget->tabCloseRequested(index);

    });

    connect(mytabwidget,&MyTabWidget::tabCloseRequested,this,[=](int index){
        if (index == 0 )
        {
            exit(0);
        }
        mytabwidget->removeTab(index);
        qlist.at(index)->setUrl(QUrl("about:blank"));
        qlist.removeAt(index);
        edit->setText(qlist.at(index-1)->url().toString());
        ui->statusbar->showMessage("加载完成");//显示message
    });

    connect(ui->forwardBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->forward();
    });
    connect(ui->backBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->back();
    });
    connect(ui->reloadBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->reload();
    });
    //---------------------------------------------------------------------------------------

    connect(ui->lineEdit,&QLineEdit::returnPressed,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->setUrl(QUrl(ui->lineEdit->text()));
    });

//    connect(mytabwidget,&MyTabWidget::currentChanged,this,[=](int index){
//        if (index >= 0 && index< qlist.length())
//            edit->setText(qlist.at(index)->url().toString());
//            emit qlist.at(index)->urlChanged(qlist.at(index)->url());
//    });


    ui->statusbar->addWidget(progress);
    progress->setStyleSheet("QProgressBar{\
                            border: none;\
                            color: white;\
                            text-align: center;\
                            background: rgb(68, 69, 73);\
                    }\
                    QProgressBar::chunk {\
                            border: none;\
                            background: rgb(0, 160, 230);}");

    connect(webview,&MyWebView::loadProgress,this,[=](int p){
        if (p ==100)
        {
            ui->statusbar->showMessage("加载完成");//显示message
        }
        else
        {
            ui->statusbar->showMessage("");
            progress->setValue(p);
        }
    });

    connect(webview,&MyWebView::urlChanged,this,[=](){
       edit->setText(webview->url().toString());
    });

    //---------------------------------------------------------------------------------------
    this->qvs.append("https://www.playm3u8.cn/jiexi.php?url="); //加入可以供视频解析的站点
    this->qvs.append("http://jx.x-99.cn/api.php?id=");
    this->qvs.append("https://www.91jxs.com/jiexi/?url=");
    this->qvs.append("https://www.ikukk.com/?url=");
    this->qvs.append("https://vip.bljiex.com/?v=");
    this->qvs.append("https://jsap.attakids.com/?url=");
    this->qvs.append("https://jx.quanmingjiexi.com/?url=");

    connect(ui->vipBtn,&QPushButton::clicked,[=](){
        int index = mytabwidget->currentIndex();
        QString prefix = parseVideo();
        QString link = ui->lineEdit->text();
        bool flag = false;
        for (int i=0;i<qvs.count();i++)
        {
            if (link.contains(qvs.at(i)))
            {
                link.replace(qvs.at(i),prefix);
                qlist.at(index)->setUrl(link);
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            qlist.at(index)->setUrl(prefix + link);
        }

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString MainWindow::parseVideo()
{   srand((unsigned long)time(NULL));

    int count = this->qvs.count();
    return this->qvs.at(rand() % count); //随机使用一个站点
}


