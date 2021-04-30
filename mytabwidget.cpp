#include "mytabwidget.h"

MyTabWidget::MyTabWidget(QWidget *parent) : QTabWidget(parent)
{
    this->setStyleSheet("QTabBar::tab { width:200px;}"); //设置tabBar的宽度固定
    this->setTabsClosable(true); //设置tabBar上右边的关闭按钮（X）显示出来
}
