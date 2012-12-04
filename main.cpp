#include "advancedtableview.h"
#include "tablemodel.h"
#include "test/autotest.h"
#include "test/testtableview.h"

//#include <QAbstractItemModel>
#include <QApplication>
//#include <QColorGroup>
#include <QDebug>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>
#include <QtGlobal>

//When enabled
//Unit Tests (TestAdvancedTableView) are executed
//When the tests are passed AdvancedTableView is executed
#define RUN_TESTS

//When enabled TestTableView
// - a non modified QTableView for testing purposses
//is executed
//#define RUN_TESTTABLEVIEW

int usageExample( QApplication& a );

int runTestTableView( QApplication& a );

int main( int argc, char *argv[] )
{

    QApplication a( argc, argv );

#ifdef RUN_TESTTABLEVIEW
    return runTestTableView( a );
#endif

    int failures = 0;

#ifdef RUN_TESTS
    failures = AutoTest::run(argc, argv);
#endif

    if ( failures == 0 ) {

#ifdef RUN_TESTS
        qDebug() << "ALL TESTS PASSED";
#endif

        return usageExample( a );

    } else {

        qCritical() << failures << " TESTS FAILED!";

        return failures;

    }

}

//A Usage Example
int usageExample( QApplication& a ) {

    //setup model/view
    TableModel myModel( 2, 6 );

    AdvancedTableView myView( 0 );

    myView.setModel( &myModel );

    //set header moveable - enables swapping of columns
    QHeaderView* hheader = myView.horizontalHeader();
    hheader->setMovable( true );

    //get QTableView column count
    QAbstractItemModel* tableModel =  myView.model();
    int columnCount = tableModel->columnCount();

//************************
// Header Row Example
//************************
//create a header row with QLineEdits
    myView.addHeaderRow();

    for ( int i = 0; i < columnCount; i++ ) {

        QLineEdit* le = new QLineEdit();
/*
        QPalette p = le->palette();
        p.setColor( QPalette::Normal, QColorGroup::Base, QColor(0,255,0) );
        le->setPalette(p);
*/
        le->setText( QString("Header ") + QString::number( i+1 ) );

        myView.setHeaderWidget( i, 0, le );

    }

    //if a widget is not needed you can set a QWidget
    //in this case the widget which was before there (QLineEdit) is deleted!
    QWidget* w = new QWidget();
    myView.setHeaderWidget( 1, 0, w );

//create another header row with QPushButtons
    myView.addHeaderRow();

    for ( int i = 0; i < columnCount; i++ ) {

        QPushButton* pb = new QPushButton();

        pb->setText( QString::number( i+1 ) );

        myView.setHeaderWidget( i, 1, pb );

    }

//************************
// Footer Row Example
//************************
//create a footer row with QLineEdits
    myView.addFooterRow();

    for ( int i = 0; i < columnCount; i++ ) {

        QLineEdit* le = new QLineEdit();

        //le->setColor(QPalette::Base, QColor(0,255,0));//green color

        //QPallete p = statuswindow->IFOSC_Subsystem0_lineEdit->palette();

        //p.setColor(QPalette::Normal, QPalette::Base, QColor(0,255,0));//green color
/*
        QPalette p = le->palette();
        p.setColor( QPalette::Normal, QColorGroup::Base, QColor(0,255,0) );
        le->setPalette(p);
*/
        le->setText( QString("Footer ") + QString::number( i+1 ) );

        myView.setFooterWidget( i, 0, le );

    }

    //if a widget is not needed you can set a QWidget
    //in this case the widget which was before there (QLineEdit) is deleted!
    //QWidget* w = new QWidget();
    //myView.setFooterWidget( 1, 0, w );

    //create another footer row with QPushButtons
    /*myView.addFooterRow();

    for ( int i = 0; i < columnCount; i++ ) {

        QPushButton* pb = new QPushButton();

        pb->setText( QString::number( i+1 ) );

        myView.setFooterWidget( i, 1, pb );

    }*/

    myView.show();

    return a.exec();

}

int runTestTableView( QApplication& a ) {

    TableModel myTestModel( 2, 8 );

    TestTableView myView( false );

    myView.setModel( &myTestModel );

    TableModel myTestModelNew( 2, 6 );
    myView.setModel( &myTestModelNew );

    myView.show();

    return a.exec();

}

