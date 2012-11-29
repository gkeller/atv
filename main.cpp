#include "advancedtableview.h"
#include "tablemodel.h"
#include "test/autotest.h"
#include "test/testtableview.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDebug>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QTableView>

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

    //set header moveable
    QHeaderView* hheader = myView.horizontalHeader();
    hheader->setMovable( true );

    //get QTableView column count
    QAbstractItemModel* tableModel =  myView.model();
    int columnCount = tableModel->columnCount();

//create a header row with QLineEdits
    myView.addHeaderRow();

    for ( int i = 0; i < columnCount; i++ ) {

        QLineEdit* le = new QLineEdit();

        le->setText( QString::number( i+1 ) );

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

