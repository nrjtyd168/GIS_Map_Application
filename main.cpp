#include "mainwindow.h"
#include "ArcGISRuntimeEnvironment.h"

#include <QApplication>

using namespace Esri::ArcGISRuntime;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    const QString apiKey = QString("AAPK93639085e57e4c889d028ed40d239f2aH-gjzQhQWnPbsKiDQNkepKiZI7k9fwI3WaNhJlrtGhRn_XtlEM8SBlKEUCe0QDKv");
    if (apiKey.isEmpty())
    {
        qWarning() << "Use of Esri location services, including basemaps, requires" <<
                      "you to authenticate with an ArcGIS identity or set the API Key property.";
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
