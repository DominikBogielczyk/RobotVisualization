TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        bluetooth.cpp \
        main.cpp

INCLUDEPATH += "C:\Users\DELL\Desktop\Dominik\SFML\SFML-2.5.1\include"

LIBS += -L"C:\Users\DELL\Desktop\Dominik\SFML\SFML-2.5.1\lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

LIBS += -lOpenGL32 -lglu32

HEADERS += \
    bluetooth.h \
    headers.h
