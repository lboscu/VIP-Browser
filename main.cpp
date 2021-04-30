#include "mainwindow.h"

#include <QApplication>
#include<QWebEngineSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);


    QApplication a(argc, argv);

    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    MainWindow w;
    w.show();
    return a.exec();
}
