/**
 * @file mymodel.h
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

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

//#include <QAbstractTableModel>
#include <QtCore/QModelIndex>

/**
 * Just a simple Model to populate QTableView with data.
 *
 * @class MyModel
 *
 */
class TableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel( int rowCount, int colCount, QObject *parent = 0 );

    void setSize( int rowCount, int colCount );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;

    int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;

    void test();

private:
    int mRowCount;
    int mColCount;

};

#endif // TABLEMODEL_H
