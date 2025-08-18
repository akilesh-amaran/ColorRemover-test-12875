#include <QApplication>
#include "mainwindow.h"

// Minimal main without resource dependencies
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("Color Remover");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("ImageEditor");
    
    // No icon for testing
    // app.setWindowIcon(QIcon(":/icons/app_icon.png"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
