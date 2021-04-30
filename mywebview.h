#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QObject>
#include<QWebEngineView>

class MyWebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit MyWebView(QWidget *parent = nullptr);
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType);
signals:

};

#endif // MYWEBVIEW_H
