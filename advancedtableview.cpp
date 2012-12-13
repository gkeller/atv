#include "advancedtableview.h"

#include <QAbstractItemView>
#include <QPushButton>
#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLayoutItem>
#include <QLineEdit>
#include <QMainWindow>
#include <QPainter>
#include <QRect>
#include <QResizeEvent>
#include <QRegion>
#include <QScrollBar>
#include <QVBoxLayout>

AdvancedTableView::ScrollArea::ScrollArea( QWidget *parent )
    : QScrollArea( parent ), mSize( 0, 0 )
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
    : QTableView( parent ), mHasVerticalHeader(
          false ), mPrintDebug( false ), mMarginLayout( 0 ), mHorizontalHeaderHeight( 0 ), mVertiticalHeaderWidth( 0 ), mScrollBarCornerWidgetMarginArea( new QFrame() ), mScrollBarCornerWidgetUser( 0), mHeaderLayout( NULL ), mHeaderScrollArea( NULL ), mHeaderRows( QList< QList< QWidget* > >() ), mMarginTopHeight(
          0 ), mHeaderRowHeights( QList<int>() ), mFooterLayout( NULL ), mFooterScrollArea( NULL ), mFooterRows( QList< QList< QWidget* > >() ), mMarginBottomHeight(
          0 ), mFooterRowHeights( QList<int>() )
{
    //This is a important Setting and should not be changed!!!
    //The visible Area is determined with it.
    QAbstractItemView::setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
    QAbstractItemView::setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );

    //QTableView::setStyleSheet("background-color: yellow");
    QTableView::viewport()->setStyleSheet("background-color: yellow");
    //they layout set is
    //------------------------------------------
    //| empty                 | Header         |
    //------------------------------------------
    //| vertical table header | table (spacer) |
    //------------------------------------------
    //| empty                 | Footer         |
    //------------------------------------------
    //mMarginLayout = new QGridLayout( this );

    //QPushButton* t = new QPushButton( this );
    //mScrollBarCornerWidget = new QFrame();

    //mScrollBarCornerWidget->hide();
    //QWidget* t = new QWidget();
    //t->setDisabled();
    //t->setMaximumWidth(1000);
    //t->raise();
    //QAbstractScrollArea::setCornerWidget( mScrollBarCornerWidget );

    mScrollBarCornerWidgetMarginArea->setBackgroundRole( QPalette::Window );
    mScrollBarCornerWidgetMarginArea->setAutoFillBackground(true);
    //QAbstractScrollArea::setCornerWidget( mScrollBarCornerWidgetMarginArea );

    //installEventFilter( new AdvancedTableView::EventFilter(this) );
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

    if ( !mFooterRows.isEmpty() ) {

        mFooterRows.clear();

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

    mMarginTopHeight = this->calculateHeaderHeight();

    mMarginBottomHeight = this->calculateFooterHeight();

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

QSize AdvancedTableView::getQTableViewSize() {

    //vertical header
    int width = QTableView::verticalHeader()->width() + 4; // +4 seems to be needed

    //horizontal header
    int height = QTableView::horizontalHeader()->height() + 4;

    if( QTableView::model() ) {

        for (int i = 0; i < QTableView::model()->columnCount(); i++) {

            width += QTableView::columnWidth(i); // seems to include gridline (on my machine)

        }

        for (int i = 0; i < QTableView::model()->rowCount(); i++) {

            height += QTableView::rowHeight(i);

        }

    }

    return QSize( width, height );
}

QSize AdvancedTableView::getVisibleViewportSize() {

    int width, height = -1;

    //visibleViewportHeight
    QWidget* viewport = QAbstractItemView::viewport();
    QRegion visibleRegion = viewport->visibleRegion();

    if( !visibleRegion.isEmpty() ) {

        QRect boundingRect = visibleRegion.boundingRect();

        width = boundingRect.width();
        height = boundingRect.height();

    }

    return QSize( width, height );

}

void AdvancedTableView::createHeaderLayout() {

    //first test if mHeaderScrollArea already exists
    if( !mHeaderScrollArea ) {

        //create mHeaderScrollArea
        mHeaderScrollArea = new ScrollArea( this );

        //put the widget down in the painting hirarchy
        //this is needed for the corner widget of QAbstractScrollArea
        //to paint over mHeaderScrollArea
        //mHeaderScrollArea->lower();

        mHeaderScrollArea->stackUnder( mScrollBarCornerWidgetMarginArea );


    } else {

        //delete mHeaderLayout
        for ( int rowIdx = 0; rowIdx < mHeaderRows.size(); rowIdx++ ) {

            //iterate cols
            int columnCount = mHeaderRows[0].size();

            for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

                QWidget* w = mHeaderRows[ rowIdx ][ colIdx ];

                w->hide();

                mHeaderLayout->removeWidget( w );


                mHeaderLayout->removeItem( mHeaderLayout->itemAtPosition ( rowIdx, colIdx )  );

                w->setParent( 0 );
            }

        }

        //delete Scroll Area Widget which contains mHeaderLayout
        QWidget* scrollAreaContainer = mHeaderScrollArea->widget();
        scrollAreaContainer->hide();
        delete scrollAreaContainer;

        mHeaderLayout = 0;

    }

    Q_ASSERT_X( mHeaderLayout == 0, "AdvancedTableView::createHeaderScrollArea()", "mHeaderLayout == 0");
    mHeaderLayout = new QGridLayout( this );

    //setup mHeaderLayout
    mHeaderLayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    //scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);
    mHeaderLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    //scrollAreaLayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    mHeaderLayout->setSpacing( 0 );
    mHeaderLayout->setContentsMargins( 0, 0, 0, 0 );

    //set widgets from mHeaderRows to layout

    //get dimension
    int rowCount = 0;
    int columnCount = 0;
    if(mHeaderRows.size() > 0) {
        rowCount = mHeaderRows.size();
        columnCount = mHeaderRows.at(0).size();
    }

    //-> iterate mHeaderRows
    //iterate cols
    for ( int rowIdx = 0; rowIdx < mHeaderRows.size(); rowIdx++ ) {

        //iterate cols
        //int columnCount = mHeaderRows[0].size();
        for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

            QWidget* w = mHeaderRows[ rowIdx ][ colIdx ];

            mHeaderLayout->addWidget( w, rowIdx, colIdx );

            //the widget was hidden while removing from layout
            //now it must made visible again
            w->show();

        }

    }

    QWidget* scrollAreaContainer = new QWidget();
    scrollAreaContainer->setLayout( mHeaderLayout );

    mHeaderScrollArea->setWidget( scrollAreaContainer );

    mHeaderScrollArea->viewport()->update();
    mHeaderScrollArea->horizontalScrollBar()->update();

    this->updateHeaderRows();

}

void AdvancedTableView::createHeaderRow()
{

    //create row
    int columnCount = QTableView::horizontalHeader()->count();

    QList< QWidget* > headerRow;
    for ( int currentColumn = 0; currentColumn < columnCount; currentColumn++ ) {

        QWidget* w = new QWidget();
        headerRow.append( w );

    }

    mHeaderRows.append( headerRow );

    this->createHeaderLayout();

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

void AdvancedTableView::createFooterLayout() {

    //first test if mFooterScrollArea already exists
    if( !mFooterScrollArea ) {

        //create mFooterScrollArea
        mFooterScrollArea = new ScrollArea( this );

        //put the widget down in the painting hirarchy
        //this is needed for the corner widget of QAbstractScrollArea
        //to paint over mFooterScrollArea
       //mFooterScrollArea->lower();

        //QWidget* container = new QWidget();
        //container->setLayout( mFooterLayout );
        //
        //mMarginLayout->addWidget( mFooterScrollArea, 2, 1 );

        mFooterScrollArea->stackUnder( mScrollBarCornerWidgetMarginArea );

    } else {

        //delete mFooterLayout
        for ( int rowIdx = 0; rowIdx < mFooterRows.size(); rowIdx++ ) {

            //iterate cols
            int columnCount = mFooterRows[0].size();

            for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

                QWidget* w = mFooterRows[ rowIdx ][ colIdx ];

                w->hide();

                mFooterLayout->removeWidget( w );


                mFooterLayout->removeItem( mFooterLayout->itemAtPosition ( rowIdx, colIdx )  );

                w->setParent( 0 );
            }

        }

        //delete Scroll Area Widget which contains mFooterLayout
        QWidget* scrollAreaContainer = mFooterScrollArea->widget();
        scrollAreaContainer->hide();
        delete scrollAreaContainer;

        mFooterLayout = 0;

    }

    Q_ASSERT_X( mFooterLayout == 0, "AdvancedTableView::createHeaderScrollArea()", "mFooterLayout == 0");
    mFooterLayout = new QGridLayout( this );

    //setup mFooterLayout
    mFooterLayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    //scrollAreaLayout->setSizeConstraint(QLayout::SetFixedSize);
    mFooterLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    //scrollAreaLayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    mFooterLayout->setSpacing( 0 );
    mFooterLayout->setContentsMargins( 0, 0, 0, 0 );

    //set widgets from mFooterRows to layout

    //get dimension
    int rowCount = 0;
    int columnCount = 0;
    if(mFooterRows.size() > 0) {
        rowCount = mFooterRows.size();
        columnCount = mFooterRows.at(0).size();
    }

    //-> iterate mFooterRows
    //iterate cols
    for ( int rowIdx = 0; rowIdx < mFooterRows.size(); rowIdx++ ) {

        //iterate cols
        //int columnCount = mFooterRows[0].size();
        for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

            QWidget* w = mFooterRows[ rowIdx ][ colIdx ];

            mFooterLayout->addWidget( w, rowIdx, colIdx );

            //the widget was hidden while removing from layout
            //now it must made visible again
            w->show();

        }

    }

    QWidget* scrollAreaContainer = new QWidget();
    scrollAreaContainer->setLayout( mFooterLayout );

    mFooterScrollArea->setWidget( scrollAreaContainer );

    mFooterScrollArea->viewport()->update();
    mFooterScrollArea->horizontalScrollBar()->update();

    this->updateFooterRows();

}

void AdvancedTableView::createFooterRow()
{

    //create row
    int columnCount = QTableView::horizontalHeader()->count();

    QList< QWidget* > footerRow;
    for ( int currentColumn = 0; currentColumn < columnCount; currentColumn++ ) {

        QWidget* w = new QWidget();
        footerRow.append( w );

    }

    mFooterRows.append( footerRow );

    this->createFooterLayout();

}

bool AdvancedTableView::verifyFooterIndex( int col, int row )
{

    //test if the indexes are valid
    //row
    if ( row < 0 || mFooterRows.size() < (row + 1) ) {

        return false;

    }
    //col
    if ( col < 0 || mFooterRows.at( row ).size() < (col + 1) ) {

        return false;

    }

    return true;
}

void AdvancedTableView::updateFooterRows()
{
    this->calculateFooterHeight();

    int absoluteTableSize = 0;

    //vertical header
    int verticalFooterWidth = 0;
    if ( mHasVerticalHeader ) {

        verticalFooterWidth = QTableView::verticalHeader()->sizeHint().width();

        absoluteTableSize += verticalFooterWidth;

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
        for ( int rowIdx = 0; rowIdx < mFooterRows.size(); rowIdx++ ) {

            int rowHeight = mFooterRowHeights.at( rowIdx );
            mFooterRows[ rowIdx ][ colIdx ]->setFixedSize( columnWidth, rowHeight );

        }

    }

    if ( mFooterScrollArea ) {

        QScrollBar* hsbMScrollArea = mFooterScrollArea->horizontalScrollBar();
        hsbMScrollArea->setPageStep( 1 );
        hsbMScrollArea->setMinimum( 0 );
        hsbMScrollArea->setMaximum( (absoluteTableSize - verticalFooterWidth) );

        mFooterScrollArea->setSize( QAbstractScrollArea::width(), mMarginBottomHeight );

        //QSize thisViewport = QAbstractScrollArea::viewport()->sizeHint();
        //QSize otherViewport = mHeaderScrollArea->viewport()->sizeHint();

        mFooterScrollArea->viewport()->update();
        mFooterScrollArea->horizontalScrollBar()->update();

    }

}

// als the height for each row is calculated which is stored in mFooterRowHeights
int AdvancedTableView::calculateFooterHeight()
{

    int columnCount = QTableView::horizontalHeader()->count();

    mFooterRowHeights.clear();

    int footerHeight = 0;

    //iterate rows
    for ( int rowIdx = 0; rowIdx < mFooterRows.size(); rowIdx++ ) {

        int rowHeight = 0;

        //iterate cols
        for ( int colIdx = 0; colIdx < columnCount; colIdx++ ) {

            int currentHeight = mFooterRows[ rowIdx ][ colIdx ]->sizeHint().height();

            //for each row the height of the max-height widget is taken
            if ( rowHeight < currentHeight ) {

                rowHeight = currentHeight;

            }

        }

        mFooterRowHeights.append( rowHeight );

        //summ height of all rows
        footerHeight += rowHeight;

    }

    //add one more pixel
    footerHeight++;

    return footerHeight;
}

void AdvancedTableView::calculateFooterPosition( int visibleViewportHeight ) {

    if( visibleViewportHeight >= 0 ) {

        mFooterScrollArea->move( 0, (mMarginTopHeight + mHorizontalHeaderHeight + visibleViewportHeight) );
        return;
    }


}

//setViewportMargins ( int left, int top, int right, int bottom )
void AdvancedTableView::updateGeometries()
{

    mVertiticalHeaderWidth = 0;

    if ( !QTableView::verticalHeader()->isHidden() ) {

        mVertiticalHeaderWidth = qMax( QTableView::verticalHeader()->minimumWidth(),
            QTableView::verticalHeader()->sizeHint().width() );
        mVertiticalHeaderWidth = qMin( mVertiticalHeaderWidth, QTableView::verticalHeader()->maximumWidth() );

    }

    mHorizontalHeaderHeight = 0;

    if ( !QTableView::horizontalHeader()->isHidden() ) {

        mHorizontalHeaderHeight = qMax( QTableView::horizontalHeader()->minimumHeight(),
            QTableView::horizontalHeader()->sizeHint().height() );
        mHorizontalHeaderHeight = qMin( mHorizontalHeaderHeight, QTableView::horizontalHeader()->maximumHeight() );

    }

    bool reverse = QTableView::isRightToLeft();

    if ( reverse ) {

        setViewportMargins( mVertiticalHeaderWidth, mHorizontalHeaderHeight + mMarginTopHeight, 0, 0 );
        setContentsMargins( 0, 0, 0, 0 );
//header
        if ( mHeaderScrollArea ) {

            mHeaderScrollArea->setViewportMargins( mVertiticalHeaderWidth, 0, 0, 0 );

        }
//footer
        if ( mFooterScrollArea ) {

            mFooterScrollArea->setViewportMargins( mVertiticalHeaderWidth, 0, 0, 0 );

        }

    } else {

        //int totalHeight = QTableView::totalHeight();
        QSize ts = getQTableViewSize();

        //int north = horizontalHeaderHeight + mMarginTopHeight;
        //int south = north + ts.height(), mMarginBottomHeight;

        int leftMargin = mVertiticalHeaderWidth;
        int topMargin = mHorizontalHeaderHeight + mMarginTopHeight;
        int rightMargin = 0;
        int bottomMargin = mMarginBottomHeight;

        setViewportMargins( leftMargin, topMargin, rightMargin, bottomMargin );
        //setViewportMargins( vertiticalHeaderWidth, north, 0, 0 );

        //QTableView::hide();
        //int south = height + mMarginBottomHeight;
        //setViewportMargins( width, 0, 0, south );

        //setContentsMargins( 0, 0, 0, 0 );

        //qCritical() << "updateGeometries(); " << north;
//header
        if ( mHeaderScrollArea ) {

            mHeaderScrollArea->setViewportMargins( mVertiticalHeaderWidth, 0, 0, 0 );

        }
//footer
        if ( mFooterScrollArea ) {

            mFooterScrollArea->setViewportMargins( mVertiticalHeaderWidth, 0, 0, 0 );

            QSize visibleViewportSize = this->getVisibleViewportSize();
            if( visibleViewportSize.height() >= 0 ) {

                this->calculateFooterPosition( visibleViewportSize.height() );

            }

            //visibleViewportHeight
            /*QWidget* viewport = QAbstractItemView::viewport();
            QRegion visibleRegion = viewport->visibleRegion();

            if( !visibleRegion.isEmpty() ) {

                QRect boundingRect = visibleRegion.boundingRect();

                this->calculateFooterPosition( boundingRect.height() );

            }*/

        }

    }

    QTableView::updateGeometries();
    //QTableView::repaint();

}

bool AdvancedTableView::event(QEvent *event)
{

    QTableView::event(event);

    //catch resize event
    if (event->type() == QEvent::Resize) {
        QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(event);

        //check if AdvancedTableView is resized to Margin Area Size
        QSize oldSize = resizeEvent->oldSize();
        int oldSizeHeight = oldSize.height();
        QSize newSize = resizeEvent->size();
        int newSizeHeight = newSize.height();

        int horizontalScrollBarHeight = 0;
        if( horizontalScrollBar()->isVisible() ) {
            horizontalScrollBarHeight = horizontalScrollBar()->height();
        }

        int marginHeigt = mMarginTopHeight + horizontalHeader()->size().height() + mMarginBottomHeight + horizontalScrollBarHeight;

        if( (oldSizeHeight > marginHeigt) & (newSizeHeight <= marginHeigt) ) {
            //enter Margin Area Size

            QWidget* cornerWidgetUser = QAbstractScrollArea::cornerWidget();

            //store user scroll bar corner widget
            if( cornerWidgetUser ) {

                mScrollBarCornerWidgetUser = cornerWidgetUser;

            }

            //set mScrollBarCornerWidgetMarginArea
            QTableView::setCornerWidget( mScrollBarCornerWidgetMarginArea );

            mFooterScrollArea->stackUnder( mScrollBarCornerWidgetMarginArea );
            mFooterScrollArea->repaint();


            mScrollBarCornerWidgetMarginArea->show();

        }

        if( (oldSizeHeight <= marginHeigt) & (newSizeHeight > marginHeigt) ) {
            //leave Margin Area Size

            //restore user scroll bar corner widget
            if( mScrollBarCornerWidgetUser ) {

                QAbstractScrollArea::setCornerWidget( mScrollBarCornerWidgetUser );
                mScrollBarCornerWidgetUser->show();

            } else {

                //clear
                //QAbstractScrollArea::setCornerWidget( 0 );


            }
        }

    }

}

void AdvancedTableView::onQTableViewHHGeometriesChanged()
{

    this->updateHeaderRows();

    this->updateFooterRows();

}

void AdvancedTableView::onQTableViewHHSectionResized( int logicalIndex, int oldSize, int newSize )
{

    if ( this->mPrintDebug )
        qDebug() << "DBFilterUI::onQTableViewHHSectionResized( " << logicalIndex << ", " << oldSize
            << ", " << newSize << ")";

    QTableView::viewport()->update();
    QTableView::horizontalScrollBar()->update();

    this->updateHeaderRows();

    this->updateFooterRows();

}

void AdvancedTableView::onQTableViewHHSectionCountChanged( int oldCount, int newCount )
{

    if ( this->mPrintDebug ) {

        qDebug() << "DBFilterUI::onQTableViewHHSectionCountChanged( " << oldCount << ", "
            << newCount << ")";

    }

    this->updateHeaderRows();

    this->updateFooterRows();

}
void AdvancedTableView::onQTableViewHHSectionMoved( int logicalIndex, int oldVisualIndex,
    int newVisualIndex )
{

    if ( this->mPrintDebug ) {

        qDebug() << "DBFilterUI::onQTableViewHHSectionMoved( " << logicalIndex << ", "
            << oldVisualIndex << ", " << newVisualIndex << ")";

    }

//swap widgets in mHeaderRows
    for( int row=0; row < mHeaderRows.count(); row++ ) {

        //store new position
        QWidget* storedWidget = mHeaderRows[row][newVisualIndex];
        //set new position
        mHeaderRows[row][newVisualIndex] = mHeaderRows[row][oldVisualIndex];
        //set old position
        mHeaderRows[row][oldVisualIndex] = storedWidget;

    }

    this->createHeaderLayout();

    this->updateHeaderRows();

//swap widgets in mFooterRows
    for( int row=0; row < mFooterRows.count(); row++ ) {

        //store new position
        QWidget* storedWidget = mFooterRows[row][newVisualIndex];
        //set new position
        mFooterRows[row][newVisualIndex] = mFooterRows[row][oldVisualIndex];
        //set old position
        mFooterRows[row][oldVisualIndex] = storedWidget;

    }

    this->createFooterLayout();

    this->updateFooterRows();
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

bool AdvancedTableView::addFooterRow()
{

    //test if a model was already set to te view
    if ( !selectionModel() ) {

        return false;

    }

    this->createFooterRow();

    return true;

}

int AdvancedTableView::getFooterRowCount()
{

    return mFooterRows.size();

}

bool AdvancedTableView::setFooterWidget( int col, int row, QWidget* widget )
{

    if ( !this->verifyFooterIndex( col, row ) ) {

        return false;

    }

    QWidget* oldWidget = mFooterRows[ row ][ col ];

    if ( oldWidget ) {

        delete oldWidget;
        oldWidget = 0;

    }

    mFooterRows[ row ][ col ] = widget;

    mFooterLayout->addWidget( widget, row, col );

    mMarginBottomHeight = AdvancedTableView::calculateFooterHeight();

    mFooterScrollArea->viewport()->update();
    mFooterScrollArea->horizontalScrollBar()->update();

    return true;

}

QWidget* AdvancedTableView::getFooterWidget( int col, int row )
{

    if ( !this->verifyFooterIndex( col, row ) ) {

        return 0;

    }

    return mFooterRows[ row ][ col ];

}
/*
void AdvancedTableView::resize ( int w, int h ) {
    qDebug() << "hhh";
    resize ( w, h );
}

void AdvancedTableView::resize ( const QSize & s ) {
    this->resize( s.width(), s.height() );
}
*/
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

    if ( mFooterScrollArea ) {

        QScrollBar* horizontalScrollBarFooter = mFooterScrollArea->horizontalScrollBar();

        //int pageStep = horizontalScrollBarFooter->pageStep();

        int valueFooter = horizontalScrollBarFooter->value();

        //int min = horizontalScrollBarFooter->minimum();
        //int max = horizontalScrollBarFooter->maximum();

        horizontalScrollBarFooter->setValue( (valueFooter - dx) );

    }
}

void AdvancedTableView::paintEvent( QPaintEvent * event )
{

    if ( mHeaderScrollArea ) {

        mHeaderScrollArea->setFixedSize( QAbstractScrollArea::width(), mMarginTopHeight );

    }

    if ( mFooterScrollArea ) {

        mFooterScrollArea->setFixedSize( QAbstractScrollArea::width(), mMarginBottomHeight );

    }



    QTableView::paintEvent( event );

}

void AdvancedTableView::resizeEvent( QResizeEvent * event ) {

    QTableView::resizeEvent( event );

    if( event->isAccepted() ) {

        //recalculate footer position
        if ( mFooterScrollArea ) {

            int visibleViewportHeight = event->size().height();

            this->calculateFooterPosition( visibleViewportHeight );

        }

    }

}
