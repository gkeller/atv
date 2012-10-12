#include "testtableview.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayoutItem>
#include <QLineEdit>
#include <QScrollBar>
#include <QVBoxLayout>

TestTableView::TestTableView( QWidget *parent )
    : QTableView( parent )
{

}

TestTableView::~TestTableView()
{

}

void TestTableView::setModel( QAbstractItemModel* model )
{

    QTableView::verticalHeader()->setModel( model );
    QTableView::horizontalHeader()->setModel( model );
    QAbstractItemView::setModel( model );

    if ( model ) {
        //QAbstractItemModel signals
        QObject::connect( QTableView::model(), SIGNAL( modelAboutToBeReset() ), this,
            SLOT( onModelChanged() ) );
    }

}

void TestTableView::onModelChanged()
{

}
