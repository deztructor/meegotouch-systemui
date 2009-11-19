include(duiconfig.pri)
STUB_PREFIX = ../tests/stubs/
TEMPLATE = app
#DEFINES += UTILISE_BATTERY_USER
TARGET = sysuid
target.path = /usr/bin
CONFIG += dui \
    cellular-qt \
    qmsystem
QT += dbus
LIBS += -lX11 \
    -lcallui0 \
    -lhal
INCLUDEPATH += /usr/include/alarmd \
    /usr/include/dbus-1.0 \
    /usr/lib/dbus-1.0/include
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
SOURCES += main.cpp \
    notifier.cpp \
    batterybusinesslogic.cpp \
    pincodequeryui.cpp \
    pincodequerybusinesslogic.cpp \
    sysuid.cpp \
    lockscreenbusinesslogic.cpp \
    lockscreenui.cpp \
    eventhandler.cpp \
    shutdownui.cpp \
    shutdownbusinesslogic.cpp \
    batterybusinesslogicadaptor.cpp \    
    systemuigconf.cpp \    
    notifierdbusadaptor.cpp \
    pincodequerydbusadaptor.cpp \
    $$STUB_PREFIX/devicemodestub.cpp \
    $$STUB_PREFIX/batterystub.cpp \
    $$STUB_PREFIX/displaystatestub.cpp \
    $$STUB_PREFIX/ledstub.cpp \
    lockscreenbusinesslogicadaptor.cpp

HEADERS += main.h \
    notifier.h \
    batterybusinesslogic.h \
    pincodequeryui.h \
    pincodequerybusinesslogic.h \
    sysuid.h \
    lockscreenbusinesslogic.h \
    lockscreenui.h \
    eventhandler.h \
    shutdownui.h \
    shutdownbusinesslogic.h \
    batterybusinesslogicadaptor.h \    
    systemuigconf.h \    
    notifierdbusadaptor.h \        
    pincodequerydbusadaptor.h \
    notificationtype.h \
    $$STUB_PREFIX/devicemodestub.h \
    $$STUB_PREFIX/batterystub.h \
    $$STUB_PREFIX/displaystatestub.h \    
    $$STUB_PREFIX/ledstub.h \
    lockscreenbusinesslogicadaptor.h

style_sheet.files += sysuid.css
style_svg.files += *.svg \
    ../data/*.svg
include(unlocksliderwidget/unlocksliderwidget.pri)
style_sheet.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/style
style_svg.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/svg
style_images.path = $$(DEBIAN_DESTDIR)/usr/share/sysuid/themes/images
INSTALLS += target \
    style_sheet \
    style_svg \
    style_images
