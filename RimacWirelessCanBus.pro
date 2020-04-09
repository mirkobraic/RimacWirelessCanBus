QT += quick concurrent

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CanLayer/KvaserWirelessCan.cpp \
        IsotpLayer/IsotpCanProvider.cpp \
        IsotpLayer/IsotpLogger.cpp \
        IsotpLayer/IsotpManager.cpp \
        Models/CanMessage.cpp \
        Models/CanMessageListModel.cpp \
        ViewController.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    CanLayer/CanBusInterface.h \
    CanLayer/CanBusInterfaceFactory.h \
    CanLayer/KvaserWirelessCan.h \
    IsotpLayer/IsotpCanProvider.h \
    IsotpLayer/IsotpLogger.h \
    IsotpLayer/IsotpManager.h \
    Models/CanMessage.h \
    Models/CanMessageListModel.h \
    ViewController.h

LIBS += -L$$PWD/../udsclient_and_isotp/Isotp/Windows/ -lisotp_lib

INCLUDEPATH += $$PWD/../udsclient_and_isotp/Isotp/Include
DEPENDPATH += $$PWD/../udsclient_and_isotp/Isotp/Include

PRE_TARGETDEPS += $$PWD/../udsclient_and_isotp/Isotp/Windows/isotp_lib.lib


LIBS += -L$$PWD/../CANlib/Lib/x64/ -lcanlib32

INCLUDEPATH += $$PWD/../CANlib/INC
DEPENDPATH += $$PWD/../CANlib/INC

PRE_TARGETDEPS += $$PWD/../CANlib/Lib/x64/canlib32.lib


LIBS += -L$$PWD/../CANlib/Lib/x64/ -lkvrlib

INCLUDEPATH += $$PWD/../CANlib/INC
DEPENDPATH += $$PWD/../CANlib/INC

PRE_TARGETDEPS += $$PWD/../CANlib/Lib/x64/kvrlib.lib
