# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = bluetooth-messenger

CONFIG += sailfishapp
QT += bluetooth

SOURCES += \
    src/bluetooth-messenger.cpp \
    src/messengerserver.cpp \
    src/messengerclient.cpp

HEADERS += \
    src/messengerserver.h \
    src/messengerclient.h

OTHER_FILES += \
    qml/bluetooth-messenger.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/bluetooth-messenger.changes.in \
    rpm/bluetooth-messenger.spec \
    rpm/bluetooth-messenger.yaml \
    translations/*.ts \
    bluetooth-messenger.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/bluetooth-messenger-de.ts

DISTFILES += \
    qml/pages/ClientPage.qml \
    qml/pages/ServerPage.qml
