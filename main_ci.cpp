#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Color Remover");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ImageEditor");
    
    // No icon for CI builds - avoid resource dependency
    // app.setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
