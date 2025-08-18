#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "mainwindow.h"

// Use standard main instead of qMain to avoid Qt entry point issues
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Color Remover");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ImageEditor");
    
    // Set application icon
    app.setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
