#include "mytabwidget.h"

MyTabWidget::MyTabWidget(QWidget *parent) : QTabWidget(parent)
{
    this->setStyleSheet("QTabBar::tab { width:200px;}");
    this->setTabsClosable(true);
}
