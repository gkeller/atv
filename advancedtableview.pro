
QT       += core gui testlib

TARGET = advancedtableview
TEMPLATE = app

SOURCES += main.cpp\
    tablemodel.cpp \
    advancedtableview.cpp \
    test/testtableview.cpp \
    test/testadvancedtableview.cpp

HEADERS  += tablemodel.h \
    advancedtableview.h \
    test/testtableview.h \
    test/testadvancedtableview.h \
    test/autotest.h

OTHER_FILES += \
    LICENSE.txt \
    README
