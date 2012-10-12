#ifndef TESTADVANCEDTABLEVIEW_H
#define TESTADVANCEDTABLEVIEW_H

#include "advancedtableview.h"
#include "autotest.h"

#include <QFrame>

class TestAdvancedTableView: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void addHeaderRow();
    void getHeaderRowCount();

    void setHeaderWidget();
    void getHeaderWidget();

    void setModel();

    void cleanupTestCase();

private:
    AdvancedTableView* mATV;
    
};

DECLARE_TEST( TestAdvancedTableView )

#endif // TESTADVANCEDTABLEVIEW_H
