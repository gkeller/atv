
QT += core gui testlib

TARGET = advancedtableview
TEMPLATE = app

##more strict compiler warnings
#QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra -ansi -pedantic -Weffc++

##to ommit warnings from qt headers the are included as system lib (-isystem)
#QMAKE_CXXFLAGS += -isystem$$QMAKE_INCDIR_QT
##todo: find a dynamic solution to include qt libs
#QMAKE_CXXFLAGS += -isystem$$QMAKE_INCDIR_QT$$/QtCore
#QMAKE_CXXFLAGS += -isystem$$QMAKE_INCDIR_QT$$/QtGui
#QMAKE_CXXFLAGS += -isystem$$QMAKE_INCDIR_QT$$/QtTest
#QMAKE_INCDIR_QT =

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

