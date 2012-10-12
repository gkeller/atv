#include "tablemodel.h"

TableModel::TableModel( int rowCount, int colCount, QObject *parent )
    : QAbstractTableModel( parent ), mRowCount( rowCount ), mColCount( colCount )
{
}

void TableModel::setSize( int rowCount, int colCount )
{
    mRowCount = rowCount;
    mColCount = colCount;
    return;
}

int TableModel::rowCount( const QModelIndex & /*parent*/) const
{
    return mRowCount;
}

int TableModel::columnCount( const QModelIndex & /*parent*/) const
{
    return mColCount;
}

QVariant TableModel::data( const QModelIndex &index, int role ) const
{
    if ( role == Qt::DisplayRole ) {
        return QString( "Row%1, Column%2" ).arg( index.row() + 1 ).arg( index.column() + 1 );
    }
    return QVariant();
}
