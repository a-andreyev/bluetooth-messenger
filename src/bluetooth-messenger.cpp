#include <QtQuick>
#include <sailfishapp.h>
#include "messengerserver.h"
#include "messengerclient.h"


int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    qmlRegisterType<MessengerServer>("bluetooth.messenger", 1, 0, "MessengerServer");
    qmlRegisterType<MessengerClient>("bluetooth.messenger", 1, 0, "MessengerClient");
    return SailfishApp::main(argc, argv);
}
