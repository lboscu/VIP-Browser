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
    progress = new QProgressBar; // 在堆内存上，分配一个进度条
    this->showMaximized(); //窗口默认最大化打开
    mytabwidget = ui->tabWidget; // ui->tabWidget是私有属性，复制到公有的成员变量，方便访问
    edit = ui->lineEdit;  //同上
    stabar = ui->statusbar;  //同上

    ui->tabWidget->removeTab(1);  //移除两个tab以便重新添加
    ui->tabWidget->removeTab(0); 

    ui->lineEdit->setText("http://www.baidu.com/");  // 浏览器初始打开网页

    // 下面太长了看着烦，分成几个部分
    //---------------------------------------------------------------------------------------
    //  浏览器初始化，即第一个网页的一些设置
    MyWebView* webview = new MyWebView;
    qlist.append(webview); //添加视图
    webview->load(QUrl("http://www.baidu.com/")); //加载网址

    ui->tabWidget->addTab(webview,"tab"); //将webview添加到窗口的tabwidget中

    // 设置TabBar的text。注意绑定的是这个新建的view
    connect(webview,&MyWebView::titleChanged,this,[=](){
            ui->tabWidget->setTabText(0,webview->title());
    });
        
    //设置tabBar的图标
    connect(webview,&MyWebView::iconChanged,this,[=](QIcon ic){
        mytabwidget->setTabIcon(0,ic);
    });
        
    connect(webview,&MyWebView::urlChanged,this,[=](){
       edit->setText(webview->url().toString());
    });

    //---------------------------------------------------------------------------------------
    // 对于的MyTabWidget的一些映射操作
    //tabBar被左键单击两次，则关闭这个tab
    connect(mytabwidget,&MyTabWidget::tabBarDoubleClicked,this,[=](int index){
        emit mytabwidget->tabCloseRequested(index);

    });
        
    // tabBar右边的×被点击时，同样删除这个tab
    connect(mytabwidget,&MyTabWidget::tabCloseRequested,this,[=](int index){
        if (index == 0 )
        {
            exit(0);
        }
        mytabwidget->removeTab(index); // 移除这个tab。但只是隐藏
        qlist.at(index)->setUrl(QUrl("about:blank")); // 隐藏后，视图中的视频等仍在播放，更改为空链接后才算删除视图
        qlist.removeAt(index); //视图链表中，删除这个视图
        edit->setText(qlist.at(index-1)->url().toString()); // 设置blank链接会导致地址栏出现bug，这里更新一下
        ui->statusbar->showMessage("加载完成");//显示message
    });
        
    // 前进按钮
    connect(ui->forwardBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex(); // 当前在那个tab下
        qlist.at(index)->forward(); //对应tab的视图前进。因为新建tab、视图和添加视图是在同时完成的，所以视图链表存储的顺序即是tab的索引顺序
    });
    
    // 后退按钮
    connect(ui->backBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->back();
    });
        
    // 刷新按钮
    connect(ui->reloadBtn,&QPushButton::clicked,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->reload();
    });
       
    // 地址栏的文本框内回车键被按下，则将当前tab中视图的url进行修改，即刷新网页
    connect(ui->lineEdit,&QLineEdit::returnPressed,this,[=](){
        int index = mytabwidget->currentIndex();
        qlist.at(index)->setUrl(QUrl(ui->lineEdit->text()));
    });
        
    //---------------------------------------------------------------------------------------
    // 进度条的一些设置
    // 状态栏中添加进度条
    ui->statusbar->addWidget(progress);
    // 进度条的qss设置
    progress->setStyleSheet("QProgressBar{\
                            border: none;\
                            color: white;\
                            text-align: center;\
                            background: rgb(68, 69, 73);\
                    }\
                    QProgressBar::chunk {\
                            border: none;\
                            background: rgb(0, 160, 230);}");

    // 初始化视图的加载进度
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


    //---------------------------------------------------------------------------------------
    // 解析VIP视频
    // 添加站点到链表中
    this->qvs.append("https://www.playm3u8.cn/jiexi.php?url="); //加入可以供视频解析的站点
    this->qvs.append("http://jx.x-99.cn/api.php?id=");
    this->qvs.append("https://www.91jxs.com/jiexi/?url=");
    this->qvs.append("https://www.ikukk.com/?url=");
    this->qvs.append("https://vip.bljiex.com/?v=");
    this->qvs.append("https://jsap.attakids.com/?url=");
    this->qvs.append("https://jx.quanmingjiexi.com/?url=");

    // VIP按钮被按下时
    connect(ui->vipBtn,&QPushButton::clicked,[=](){
        int index = mytabwidget->currentIndex();
        QString prefix = parseVideo();
        QString link = ui->lineEdit->text(); // 获取当前网页链接
        bool flag = false;  // 标志，网页地址中是否包含站点，即是否解析过
        for (int i=0;i<qvs.count();i++)
        {
            if (link.contains(qvs.at(i))) // 是否包含解析站点
            {
                link.replace(qvs.at(i),prefix);  //包含解析站点，就替换以使用其余站点来解析
                qlist.at(index)->setUrl(link);
                flag = true;
                break;
            }
        }
        if (!flag) //表示是第一次按VIP按钮
        {
            qlist.at(index)->setUrl(prefix + link);
        }

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 随机获得一个解析视频的站点
QString MainWindow::parseVideo()
{   srand((unsigned long)time(NULL));

    int count = this->qvs.count();
    return this->qvs.at(rand() % count); //随机使用一个站点
}


