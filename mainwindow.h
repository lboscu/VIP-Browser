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
    MyTabWidget* mytabwidget;
    QLineEdit* edit;
    QList<MyWebView*> qlist;
    QProgressBar* progress;
    QStatusBar* stabar;

    QList<QString> qvs; // 可以供视频解析的网站站点
    QString parseVideo();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
