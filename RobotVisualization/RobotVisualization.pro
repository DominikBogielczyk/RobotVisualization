QT += widgets serialport
requires(qtConfig(combobox))

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle

SOURCES += \
        drawingfunctions.cpp \
        main.cpp \
        pid_controller.cpp \
        robot.cpp \
        room.cpp \
        trafficcone.cpp

INCLUDEPATH += "C:\Program Files\SFML-2.5.1\include"

LIBS += -L"C:\Program Files\SFML-2.5.1\lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

LIBS += -lOpenGL32 -lglu32

HEADERS += \
    drawingfunctions.h \
    headers.h \
    pid_controller.h \
    robot.h \
    room.h \
    trafficcone.h


