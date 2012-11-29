
QT       += core gui testlib

TARGET = advancedtableview
TEMPLATE = app

#more strict compiler warnings
#QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra -ansi -pedantic
#-Weffc++

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
