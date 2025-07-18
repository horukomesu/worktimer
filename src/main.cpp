#include <QApplication>
#include <QStyleFactory>
#include "worktimer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("WorkTimer");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("WorkTimer Team");
    
    // Create and show main window
    WorkTimer window;
    window.show();
    
    return app.exec();
} 