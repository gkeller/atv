#include "testadvancedtableview.h"

#include "tablemodel.h"

#include <QLineEdit>

void TestAdvancedTableView::initTestCase()
{
    /*
    TableModel myModel( 2, 8 );

    mATV = new AdvancedTableView( false );

    mATV->setModel( &myModel );
    */

}

void TestAdvancedTableView::cleanupTestCase()
{
    /*
    if ( mATV ) {
        delete mATV;
        mATV = 0;
    }
    */

}

void TestAdvancedTableView::addHeaderRow()
{

    TableModel myModel( 2, 8 );

    AdvancedTableView* ATV = new AdvancedTableView( false );

    QVERIFY( ATV->addHeaderRow() == false );

    ATV->setModel( &myModel );

    QVERIFY( ATV->addHeaderRow() == true );

}

void TestAdvancedTableView::getHeaderRowCount()
{

    TableModel myModel( 2, 8 );

    AdvancedTableView* ATV = new AdvancedTableView( false );

    ATV->setModel( &myModel );

    QVERIFY( ATV->getHeaderRowCount() == 0 );

    QVERIFY( ATV->addHeaderRow() == true );

    QVERIFY( ATV->getHeaderRowCount() == 1 );

}

void TestAdvancedTableView::setHeaderWidget()
{

    TableModel myModel( 2, 8 );

    AdvancedTableView* ATV = new AdvancedTableView( false );

    ATV->setModel( &myModel );

    QVERIFY( ATV->addHeaderRow() == true );

    QAbstractItemModel* tableModel = ATV->model();
    int columnCount = tableModel->columnCount();

    for ( int i = 0; i < columnCount; i++ ) {

        QLineEdit* le = new QLineEdit();

        QVERIFY( ATV->setHeaderWidget( i, 0, le ) == true );

        //compare pointers
        QVERIFY( ATV->getHeaderWidget( i, 0 ) == le );

    }

    //verify negative and too big column index
    QWidget* w = ATV->getHeaderWidget( 0, 0 );
    QVERIFY( w != 0 );
    QVERIFY( ATV->setHeaderWidget( -1, 0, w ) == false );
    QVERIFY( ATV->setHeaderWidget( columnCount, 0, w ) == false );

}

void TestAdvancedTableView::getHeaderWidget()
{
    //tested in setHeaderWidget()
}

void TestAdvancedTableView::setModel()
{

    TableModel myModel( 2, 8 );

    AdvancedTableView* ATV = new AdvancedTableView( false );

    ATV->setModel( &myModel );

    //set a header row and set some widgets
    QVERIFY( ATV->addHeaderRow() == true );

    QAbstractItemModel* tableModel = ATV->model();
    int columnCount = tableModel->columnCount();

    for ( int i = 0; i < columnCount; i++ ) {

        QLineEdit* le = new QLineEdit();

        QVERIFY( ATV->setHeaderWidget( i, 0, le ) == true );

        //compare pointers
        QVERIFY( ATV->getHeaderWidget( i, 0 ) == le );

    }

    //create a new model
    TableModel myModelNew( 2, 6 );

    //set the new model
    ATV->setModel( &myModelNew );

    //verify that no widgets are set
    QVERIFY( ATV->getHeaderRowCount() == 0 );
    QVERIFY( ATV->getHeaderWidget( 0, 0 ) == 0 );

}

