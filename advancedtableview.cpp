#include "advancedtableview.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayoutItem>
#include <QLineEdit>
#include <QScrollBar>
#include <QVBoxLayout>

AdvancedTableView::ScrollArea::ScrollArea( QWidget *parent )
    : QScrollArea( parent )
{
    //When true the scroll bar of the custom widgets is shown which is usually not shown
    //but this is sometimes usefull for debugging
    bool showScrollBar = false;

    if ( showScrollBar ) {

        //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
        QScrollBar* hsbMScrollArea = horizontalScrollBar();
        hsbMScrollArea->show();

        setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        QScrollBar* vsbMScrollArea = verticalScrollBar();
        vsbMScrollArea->hide();

    } else {

        //setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        QScrollBar* hsbMScrollArea = horizontalScrollBar();
        hsbMScrollArea->hide();

        setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        QScrollBar* vsbMScrollArea = verticalScrollBar();
        vsbMScrollArea->hide();

    }

}

void AdvancedTableView::ScrollArea::setViewportMargins( int left, int top, int right, int bottom )
{

    QScrollArea::setViewportMargins( left, top, right, bottom );

}

void AdvancedTableView::ScrollArea::setSize( int width, int height )
{

    mSize = QSize( width, height );

    updateGeometry();

}

QSize AdvancedTableView::ScrollArea::sizeHint() const
{

    return mSize;

}

AdvancedTableView::AdvancedTableView( QWidget *parent )
    : QTableView( parent ), mHeaderLayout( NULL ), mHeaderScrollArea( NULL ), mMarginTopHeight(
          0 ), mHasVerticalHeader(
        false )
{

    //print deb msg?
    mPrintDebug = false;

    QAbstractItemView::setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );

    this->setupSignalSlotConnections();

}

AdvancedTableView::~AdvancedTableView()
{

    this->clearAdvancedTableView();

}

void AdvancedTableView::clearAdvancedTableView()
{

    if ( !mHeaderRows.isEmpty() ) {

        mHeaderRows.clear();

    }

}

void AdvancedTableView::setModel( QAbstractItemModel* model ) {

    //somehow this produces a segfault?
    //todo: check the reason
    //setModel( model );

    //this is the code from QTableView::setModel( model );
    QTableView::verticalHeader()->setModel(model);
    QTableView::horizontalHeader()->setModel(model);
    QAbstractItemView::setModel(model);

    this->clearAdvancedTableView();

    mMarginTopHeight = calculateHeaderHeight();

}

void AdvancedTableView::setupSignalSlotConnections()
{

    //horizontal header signals
    QHeaderView *horizontalHeader = QTableView::horizontalHeader();

    QObject::connect(horizontalHeader, SIGNAL(sectionResized( int, int, int ) ),
        this, SLOT(onQTableViewHHSectionResized( int, int, int ) ));

    QObject::connect(horizontalHeader, SIGNAL(sectionCountChanged( int, int ) ),
        this, SLOT(onQTableViewHHSectionCountChanged( int, int ) ));

    QObject::connect(horizontalHeader, SIGNAL(sectionMoved( int, int, int ) ),
        this, SLOT(onQTableViewHHSectionMoved( int, int, int ) ));

    QObject::connect( horizontalHeader, SIGNAL( geometriesChanged() ), this,
        SLOT( onQTableViewHHGeometriesChanged() ) );

}

void AdvancedTableView::updateGeometries()
{

    int width = 0;

    if ( !QTableView::verticalHeader()->isHidden() ) {

        width = qMax( QTableView::verticalHeader()->minimumWidth(),
            QTableView::verticalHeader()->sizeHint().width() );
        width = qMin( width, QTableView::verticalHeader()->maximumWidth() );

    }

    int height = 0;

    if ( !QTableView::horizontalHeader()->isHidden() ) {

        height = qMax( QTableView::horizontalHeader()->minimumHeight(),
            QTableView::horizontalHeader()->sizeHint().height() );
        height = qMin( height, QTableView::horizontalHeader()->maximumHeight() );

    }

    bool reverse = QTableView::isRightToLeft();

    if ( reverse ) {

        setViewportMargins( width, height + mMarginTopHeight, 0, 0 );
        setContentsMargins( 0, 0, 0, 0 );

        if ( mHeaderScrollArea ) {

            mHeaderScrollArea->setViewportMargins( width, 0, 0, 0 );

        }

    } else {

        int north = height + mMarginTopHeight;
        setViewportMargins( width, north, 0, 0 );
        setContentsMargins( 0, 0, 0, 0 );

        qCritical() << "updateGeometries(); " << north;

        if ( mHeaderScrollArea ) {

            mHeaderScrollArea->setViewportMargins( width, 0, 0, 0 );

        }

    }

    QTableView::updateGeometries();

}

void AdvancedTableView::onQTableViewHHGeometriesChanged()
{

    this->updateHeaderRows();

}

void AdvancedTableView::onQTableViewHHSectionResized( int logicalIndex, int oldSize, int newSize )
{

    if ( this->mPrintDebug )
        qDebug() << "DBFilterUI::onQTableViewHHSectionResized( " << logicalIndex << ", " << oldSize
            << ", " << newSize << ")";

    QTableView::viewport()->update();
    QTableView::horizontalScrollBar()->update();

    this->updateHeaderRows();

}

void AdvancedTableView::onQTableViewHHSectionCountChanged( int oldCount, int newCount )
{

    if ( this->mPrintDebug ) {

        qDebug() << "DBFilterUI::onQTableViewHHSectionCountChanged( " << oldCount << ", "
            << newCount << ")";

    }

    this->updateHeaderRows();

}
void AdvancedTableView::onQTableViewHHSectionMoved( int logicalIndex, int oldVisualIndex,
    int newVisualIndex )
{

    if ( this->mPrintDebug ) {

        qDebug() << "DBFilterUI::onQTableViewHHSectionMoved( " << logicalIndex << ", "
            << oldVisualIndex << ", " << newVisualIndex << ")";

    }

    this->updateHeaderRows();

}

void AdvancedTableView::createHeaderRow()
{

    //if this is the first -> create header scrollarea and layout
    if ( mHeaderRows.size() == 0 ) {

        //create scrollarea
        mHeaderScrollArea = new ScrollArea( this );

        //create layout
        mHeaderLayout = new QGridLayout();
        mHeaderLayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
        //scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);
        mHeaderLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
        //scrollAreaLayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        mHeaderLayout->setSpacing( 0 );
        mHeaderLayout->setContentsMargins( 0, 0, 0, 0 );

        QWidget* scrollAreaContainer = new QWidget();
        scrollAreaContainer->setLayout( mHeaderLayout );

        //create row
        int columnCount = QTableView::horizontalHeader()->count();

        QList< QWidget* > headerRow;
        for ( int currentColumn = 0; currentColumn < columnCount; currentColumn++ ) {

            QWidget* w = new QWidget();
            mHeaderLayout->addWidget( w, 0, currentColumn );
            headerRow.append( w );

        }

        mHeaderRows.append( headerRow );

        mHeaderScrollArea->setWidget( scrollAreaContainer );

        mHeaderScrollArea->viewport()->update();
        mHeaderScrollArea->horizontalScrollBar()->update();

        this->updateHeaderRows();

    } else {

        //create header scrollarea and layout already created!
        //create row
        int columnCount = QTableView::horizontalHeader()->count();

        QList< QWidget* > headerRow;
        for ( int currentColumn = 0; currentColumn < columnCount; currentColumn++ ) {

            QWidget* w = new QWidget();
            mHeaderLayout->addWidget( w, 0, currentColumn );
            headerRow.append( w );

        }

        mHeaderRows.append( headerRow );

        mHeaderScrollArea->viewport()->update();
        mHeaderScrollArea->horizontalScrollBar()->update();

        this->updateHeaderRows();
    }

}

bool AdvancedTableView::verifyHeaderIndex( int col, int row )
{

    //test if the indexes are valid
    //row
    if ( row < 0 || mHeaderRows.size() < (row + 1) ) {

        return false;

    }
    //col
    if ( col < 0 || mHeaderRows.at( row ).size() < (col + 1) ) {

        return false;

    }

    return true;
}

bool AdvancedTableView::addHeaderRow()
{

    //test if a model was already set to te view
    if ( !selectionModel() ) {

        return false;

    }

    this->createHeaderRow();

    return true;

}

int AdvancedTableView::getHeaderRowCount()
{

    return mHeaderRows.size();

}

bool AdvancedTableView::setHeaderWidget( int col, int row, QWidget* widget )
{

    if ( !this->verifyHeaderIndex( col, row ) ) {

        return false;

    }

    QWidget* oldWidget = mHeaderRows[ row ][ col ];

    if ( oldWidget ) {

        delete oldWidget;
        oldWidget = 0;

    }

    mHeaderRows[ row ][ col ] = widget;

    mHeaderLayout->addWidget( widget, row, col );

    mMarginTopHeight = AdvancedTableView::calculateHeaderHeight();

    mHeaderScrollArea->viewport()->update();
    mHeaderScrollArea->horizontalScrollBar()->update();

    return true;

}

QWidget* AdvancedTableView::getHeaderWidget( int col, int row )
{

    if ( !this->verifyHeaderIndex( col, row ) ) {

        return 0;

    }

    return mHeaderRows[ row ][ col ];

}

void AdvancedTableView::updateHeaderRows()
{
    this->calculateHeaderHeight();

    int absoluteTableSize = 0;

    //vertical header
    int verticalHeaderWidth = 0;
    if ( mHasVerticalHeader ) {

        verticalHeaderWidth = QTableView::verticalHeader()->sizeHint().width();

        absoluteTableSize += verticalHeaderWidth;

    }

    //columns
    int columnStartIdx = 0;
    if ( mHasVerticalHeader ) {

        columnStartIdx = 1;

    }

    int columnCount = QTableView::horizontalHeader()->count();

    //iterate cols
    for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

        //wie gross ist jede spalte?
        int columnWidth = QTableView::columnWidth( colIdx );

        absoluteTableSize += columnWidth;

        //iterate rows
        for ( int rowIdx = 0; rowIdx < mHeaderRows.size(); rowIdx++ ) {

            int rowHeight = mHeaderRowHeights.at( rowIdx );
            mHeaderRows[ rowIdx ][ colIdx ]->setFixedSize( columnWidth, rowHeight );

        }

    }

    if ( mHeaderScrollArea ) {

        QScrollBar* hsbMScrollArea = mHeaderScrollArea->horizontalScrollBar();
        hsbMScrollArea->setPageStep( 1 );
        hsbMScrollArea->setMinimum( 0 );
        hsbMScrollArea->setMaximum( (absoluteTableSize - verticalHeaderWidth) );

        mHeaderScrollArea->setSize( QAbstractScrollArea::width(), mMarginTopHeight );

        //QSize thisViewport = QAbstractScrollArea::viewport()->sizeHint();
        //QSize otherViewport = mHeaderScrollArea->viewport()->sizeHint();

        mHeaderScrollArea->viewport()->update();
        mHeaderScrollArea->horizontalScrollBar()->update();

    }

}

// als the height for each row is calculated which is stored in mHeaderRowHeights
int AdvancedTableView::calculateHeaderHeight()
{

    int columnCount = QTableView::horizontalHeader()->count();

    mHeaderRowHeights.clear();

    int headerHeight = 0;

    //iterate rows
    for ( int rowIdx = 0; rowIdx < mHeaderRows.size(); rowIdx++ ) {

        int rowHeight = 0;

        //iterate cols
        for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

            int currentHeight = mHeaderRows[ rowIdx ][ colIdx ]->sizeHint().height();

            //for each row the height of the max-height widget is taken
            if ( rowHeight < currentHeight ) {

                rowHeight = currentHeight;

            }

        }

        mHeaderRowHeights.append( rowHeight );

        //summ height of all rows
        headerHeight += rowHeight;

    }

    //add one more pixel
    headerHeight++;

    return headerHeight;
}

void AdvancedTableView::scrollContentsBy( int dx, int dy )
{
    //qDebug() << "TableView::scrollContentsBy(" << dx << "," << dy << ")";

    QTableView::scrollContentsBy( dx, dy );

    if ( mHeaderScrollArea ) {

        QScrollBar* horizontalScrollBarHeader = mHeaderScrollArea->horizontalScrollBar();

        //int pageStep = horizontalScrollBarHeader->pageStep();

        int valueHeader = horizontalScrollBarHeader->value();

        //int min = horizontalScrollBarHeader->minimum();
        //int max = horizontalScrollBarHeader->maximum();

        horizontalScrollBarHeader->setValue( (valueHeader - dx) );

    }

}

void AdvancedTableView::paintEvent( QPaintEvent * event )
{

    if ( mHeaderScrollArea ) {

        mHeaderScrollArea->setFixedSize( QAbstractScrollArea::width(), mMarginTopHeight );

    }

    QTableView::paintEvent( event );

}
