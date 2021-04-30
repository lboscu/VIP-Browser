#include "mywebview.h"
#include"mainwindow.h"

MyWebView::MyWebView(QWidget *parent) : QWebEngineView(parent)
{

}


QWebEngineView* MyWebView::createWindow(QWebEnginePage::WebWindowType type)
{
    MainWindow *mainWindow = qobject_cast<MainWindow*>(window());
    if (!mainWindow)
        return nullptr;

    switch (type)
    {
        case QWebEnginePage::WebBrowserTab:
        {
            MyWebView* webview = new MyWebView;
            mainWindow->mytabwidget->addTab(webview,"");
            int index = mainWindow->mytabwidget->count();
            mainWindow->mytabwidget->setCurrentIndex(index-1);

            connect(webview,&MyWebView::titleChanged,this,[=](QString qs){
                mainWindow->mytabwidget->setTabText(index-1,qs);
            });

            connect(webview,&MyWebView::iconChanged,this,[=](QIcon ic){
                mainWindow->mytabwidget->setTabIcon(index-1,ic);
            });

            connect(webview,&MyWebView::urlChanged,this,[=](){
               mainWindow->edit->setText(webview->url().toString());
               emit webview->titleChanged(webview->title());
               emit webview->iconChanged(webview->icon());
            });

            connect(webview,&MyWebView::loadProgress,this,[=](int p){
                if (p ==100)
                {
                    mainWindow->stabar->showMessage("加载完成");//显示message
                }
                else
                {
                    mainWindow->stabar->showMessage("");
                    mainWindow->progress->setValue(p);
                }
            });

            mainWindow->qlist.append(webview);
            return webview;
        }
        case QWebEnginePage::WebBrowserBackgroundTab:
        {
            return this;
        }
        case QWebEnginePage::WebBrowserWindow:
        {
            return this;
        }
    case QWebEnginePage::WebDialog: {

        return this;
    }
    }
    return this;
}
