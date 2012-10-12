/**
 * @file testtableview.h
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

#ifndef TESTTABLEVIEW_H
#define TESTTABLEVIEW_H

#include <QScrollArea>
#include <QTableView>
#include <QVBoxLayout>

/**
 * This is a non modified QTableView for testing purposses
 * @class TestTableView
 *
 */
class TestTableView: public QTableView
{
    Q_OBJECT

public:
    /**
     *
     * @param showFilterScrollBar
     * @param printDebug
     * @param parent
     */
    explicit TestTableView( QWidget *parent = false );
    ~TestTableView();

    /**
     * Overwritten Base Function
     * The main Goal is to recognize model changes.
     * When a existing model changes all widgets are deinitilized.
     */
    void setModel( QAbstractItemModel* model );

    void onModelChanged();

};

#endif // TESTTABLEVIEW_H
