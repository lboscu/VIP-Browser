#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"mywebview.h"
#include"mytabwidget.h"
#include<QTabWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QList>
#include<QProgressBar>
#include<QStatusBar>
#include<time.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 这几个成员需要能够在MyWebView类中访问，所以需要新建这样的成员变量
    MyTabWidget* mytabwidget; //mytabwidget
    QLineEdit* edit;
    QList<MyWebView*> qlist; //存放MyWebView视图
    QProgressBar* progress;  // 进度条类
    QStatusBar* stabar; //状态栏

    QList<QString> qvs; // 可以供视频解析的网站站点
    QString parseVideo();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
