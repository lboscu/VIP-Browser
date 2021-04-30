#include "mywebview.h"
#include"mainwindow.h"

MyWebView::MyWebView(QWidget *parent) : QWebEngineView(parent)
{

}

//这个函数是重写，参数类型这些都不能修改
QWebEngineView* MyWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    //  需要在头文件中，包含#include"mainwindow.h",才能使用MainWindow这个类。
	//  this->window(),返回的是视图所在的窗口，这个很重要，有了这个可以实现一些数据的交互
    MainWindow *mainWindow = qobject_cast<MainWindow*>(window());
    if (!mainWindow)
        return nullptr;

    switch (type)
    {
        case QWebEnginePage::WebBrowserTab: // html网页请求的是打开一个新的tab
        {
            // 需要包含MyWebView类的头文件；创建一个新视图
            MyWebView* webview = new MyWebView;
            // 在窗口的mytabwidget（成员变量）添加tab，tab的视图为这个新视图
            mainWindow->mytabwidget->addTab(webview,"");
            // 获得当前mytabwidget中的tab数目，即打开了多少个网页
            int index = mainWindow->mytabwidget->count();
            // 将刚添加进来的tab设置为当前的
            mainWindow->mytabwidget->setCurrentIndex(index-1);
            // 这个信号槽是绑定视图的title和mytabwidget对应的tabBar的文字内容
            connect(webview,&MyWebView::titleChanged,this,[=](QString qs){
                mainWindow->mytabwidget->setTabText(index-1,qs);
            });
            // 这个信号槽是绑定视图的图标和mytabwidget对应的tabBar的图标
            connect(webview,&MyWebView::iconChanged,this,[=](QIcon ic){
                mainWindow->mytabwidget->setTabIcon(index-1,ic);
            });
            // 这个信号槽是绑定视图的网页地址和地址栏（lineEdit）的文本内容
            connect(webview,&MyWebView::urlChanged,this,[=](){
               mainWindow->edit->setText(webview->url().toString());
               emit webview->titleChanged(webview->title());
               emit webview->iconChanged(webview->icon());
            });
            // 这个信号槽使绑定视图的网页加载进度和状态栏的进度显示
            connect(webview,&MyWebView::loadProgress,this,[=](int p){
                if (p ==100) // 加载完成
                {
                    mainWindow->stabar->showMessage("加载完成");//显示message
                }
                else
                {
                    mainWindow->stabar->showMessage(""); //这个不能省略，作用是先屏蔽文字内容，否则不能显示进度条的进度信息
                    mainWindow->progress->setValue(p);  //progress成员变量，进度条
                }
            });
            // qlist是QList<_T>的链表，用来存储视图webview，否则便不能再mainwindow中处理视图
            mainWindow->qlist.append(webview);
            return webview; //返回新建的视图
        }
        case QWebEnginePage::WebBrowserBackgroundTab: 
        {
            return this;
        }
        case QWebEnginePage::WebBrowserWindow:  // html网页请求的是打开一个新的窗口，这里没有处理，后续可以完善
        {
            return this;
        }
    case QWebEnginePage::WebDialog: {  // html网页请求的是打开一个对话框，这里没有处理，后续可以完善

        return this;
    }
    }
    return this;
}
