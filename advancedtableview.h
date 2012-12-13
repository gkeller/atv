/**
 * @file advancedtableview.h
 *
 * @brief
 *
 * License: Simplified BSD License - see license.txt
 * Licensed Material - Property of Gerd Keller
 * (c) Copyright Gerd Keller
 *
 * @author Gerd Keller
 * @date 22.09.2012
 * @version 0.1
 */

#ifndef ADVANCEDTABLEVIEW_H
#define ADVANCEDTABLEVIEW_H

#include <qglobal.h>
#include <QFrame>
#include <QGridLayout>
//#include <QLabel>
#include <QScrollArea>
#include <QSize>
#include <QTableView>
#include <QVBoxLayout>

/**
 * Main class extends QTableView
 * When the destructor is called or a new model is set all widgets which had been set are deinitialized.
 * Memory management is done inside AdvancedTableView: When AdvancedTableView is deallocated (or the assoziated model changes) also the set Widgets are deallocated.
 *
 * @class AdvancedTableView
 *
 */
class AdvancedTableView : public QTableView
{
    Q_OBJECT

private:

    /**
     * Scroll Area for the additional Widgets extends QScrollArea
     *
     * It is needed to keep the additional Widgets in sync with the QAbstractScrollArea of QTableView.
     *
     * @class ScrollArea
     *
     */
    class ScrollArea : public QScrollArea
    {
        friend class AdvancedTableView;

    public:
        explicit ScrollArea( QWidget *parent = 0 );

        //calls protected base
        void setViewportMargins( int left, int top, int right, int bottom );

        /**
         * Sets a fixed size for this Scroll Area.
         * @param w
         * @param h
         */
        void setSize( int width, int height );

        /**
         * Implements a custom size hint.
         * @return
         */
        QSize sizeHint() const;

        void updateAdvancedTableViewWidgets();

    private:
        QSize mSize;

    };

    /**
     * Scroll Area for the additional Widgets extends QScrollArea
     *
     * It is needed to keep the additional Widgets in sync with the QAbstractScrollArea of QTableView.
     *
     * @class ScrollArea
     *
     */
    /*class EventFilter : public QObject {
        Q_OBJECT
    public:
        explicit EventFilter(QObject* target) : mTarget(target) {}
        bool eventFilter(QObject *object, QEvent *event)
        {
            if (object == mTarget) {
                return true;

            }
            return false;
        }
    private:
        QObject* mTarget;
    };*/

public:
    /**
     *
     * @param parent
     */
    AdvancedTableView( QWidget *parent );
    ~AdvancedTableView();

    /**
     * Overwritten Base Function
     * The main Goal is to recognize model changes.
     * When a existing model changes all widgets are deinitilized.
     */
    void setModel( QAbstractItemModel* model );

    /**
     * Overwritten Base Function
     * The ViewportMargins of the Scroll Areas are set here.
     */
    void updateGeometries();

    //catches QResizeEvent and handels the swapping of an user set Corner Widget with the Margin Area Corner Widget
    bool event(QEvent *event);

//Header Row Public API Interface

    /**
     * Creates a new Header Row based on the columns of the table.
     * Remark: A model must be set to the view before.
     * @return true: succesful added a Header Row / false: if no model was set to the view
     */
    bool addHeaderRow();

    /**
     * Returns the header Row count.
     * @return
     */
    int getHeaderRowCount();

    /**
     * The row and column indexes start with 0.
     * @param col Table Column [0 ... (columnCount-1)]
     * @param row Header Row [0 ... (rowCount-1)]
     * @param w
     * @return true: indexes are valid -> widget set succesful / false: indexes are not valid! -> widget was not set
     */
    bool setHeaderWidget( int col, int row, QWidget* w );

    /**
     *
     * @param col Table Column
     * @param row Header Row
     * @return
     */
    QWidget* getHeaderWidget( int col, int row );

//Footer Row Public API Interface

    /**
     * Creates a new Footer Row based on the columns of the table.
     * Remark: A model must be set to the view before.
     * @return true: succesful added a Footer Row / false: if no model was set to the view
    */
    bool addFooterRow();

    /**
     * Returns the Footer Row count.
     * @return
    */
    int getFooterRowCount();

    /**
     * The row and column indexes start with 0.
     * @param col Table Column [0 ... (columnCount-1)]
     * @param row Header Row [0 ... (rowCount-1)]
     * @param w
     * @return true: indexes are valid -> widget set succesful / false: indexes are not valid! -> widget was not set
     */
    bool setFooterWidget( int col, int row, QWidget* w );

    /**
     *
     * @param col Table Column
     * @param row Footer Row
     * @return
    */
    QWidget* getFooterWidget( int col, int row );

    //void resize ( int w, int h );
    //void resize ( const QSize & s );

public slots:

    //QTableView horizontalHeader signals
    void onQTableViewHHGeometriesChanged();
    void onQTableViewHHSectionResized( int logicalIndex, int oldSize, int newSize );
    void onQTableViewHHSectionCountChanged( int oldCount, int newCount );
    void onQTableViewHHSectionMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex );

protected:
    /**
     * Overwritten Base Function
     * Catches QTableView scroll events
     * @param dx
     * @param dy
     */
    void scrollContentsBy( int dx, int dy );

    /**
     * Overwritten Base Function
     * Triggers resize/repaint of the custom widgets
     * @param event
     */
    void paintEvent( QPaintEvent * event );

    void resizeEvent( QResizeEvent * event );

private:

//general private functions

    //disable copy constructor - this is a basic concept in Qt Widgets
    Q_DISABLE_COPY( AdvancedTableView )

    void clearAdvancedTableView();

    void setupSignalSlotConnections();

    //the size of the table cells (includes the invisible part)
    QSize getQTableViewSize();

    QSize getVisibleViewportSize();

//Header Row private functions

    //if the layout geometry of the widget container mHeaderRows changed
    //- new rows added
    //- columns swapped
    //this should be called.
    //it recreates mHeaderScrollArea
    void createHeaderLayout();

    void createHeaderRow();

    bool verifyHeaderIndex( int col, int row );

    void updateHeaderRows();

    int calculateHeaderHeight();

//Footer Row private functions

    //if the layout geometry of the widget container mHeaderRows changed
    //- new rows added
    //- columns swapped
    //this should be called.
    //it recreates mFooterScrollArea
    void createFooterLayout();

    void createFooterRow();

    bool verifyFooterIndex( int col, int row );

    void updateFooterRows();

    int calculateFooterHeight();

    void calculateFooterPosition( int visibleViewportHeight );

//general private members

    bool mHasVerticalHeader;

    bool mPrintDebug;

    //contains the ScrollAreas for Header, Footer etc
    //and a Spacer for the table
    //it is used to set the right position for the ScrollAreas
    QGridLayout* mMarginLayout;

    int mHorizontalHeaderHeight;
    int mVertiticalHeaderWidth;

    QFrame* mScrollBarCornerWidgetMarginArea; //ScrollBarCornerWidget which is shown while only the Margin Area is visible
    QWidget* mScrollBarCornerWidgetUser; //When the user has set a custom ScrollBarCornerWidget it is temporaly saved here while mScrollBarCornerWidgetMarginArea is visible.

//Header Row private members

    QGridLayout* mHeaderLayout;

    ScrollArea* mHeaderScrollArea;

    QList< QList< QWidget* > > mHeaderRows; //Container for the Header Row widgets

    int mMarginTopHeight; //height of the QTableView top margin area

    QList< int > mHeaderRowHeights; //height of each row

//Footer Row private members

    QGridLayout* mFooterLayout;

    ScrollArea* mFooterScrollArea;

    QList< QList< QWidget* > > mFooterRows; //Container for the Header Row widgets

    int mMarginBottomHeight; //height of the QTableView bottom margin area

    QList< int > mFooterRowHeights; //height of each row

};

#endif // ADVANCEDTABLEVIEW_H
