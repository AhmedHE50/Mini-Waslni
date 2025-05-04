QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17  # Or a higher C++ standard

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Source files
SOURCES += \
    FileManager.cpp \
    Graph.cpp \
    GraphOperationsWindow.cpp \
    GraphTraversalWindow.cpp \
    MainWindow.cpp \
    ShortestPathWindow.cpp \
    main.cpp

# Header files
HEADERS += \
    FileManager.h \
    Graph.h \
    GraphOperationsWindow.h \
    GraphTraversalWindow.h \
    MainWindow.h \
    ShortestPathWindow.h \
    json.hpp

FORMS += \
    GraphOperationsWindow.ui \
    GraphTraversalWindow.ui \
    MainWindow.ui \
    ShortestPathWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
