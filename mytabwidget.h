#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QWidget>
#include<QTabWidget>
#include"mywebview.h"
#include<QTabBar>

class MyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MyTabWidget(QWidget *parent = nullptr);

signals:

};

#endif // MYTABWIDGET_H
