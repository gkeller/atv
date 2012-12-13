#ifndef TESTADVANCEDTABLEVIEW_H
#define TESTADVANCEDTABLEVIEW_H

#include "autotest.h"

class TestAdvancedTableView: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void addHeaderRow();
    void getHeaderRowCount();

    void setHeaderWidget();
    void getHeaderWidget();

    void addFooterRow();
    void getFooterRowCount();

    void setFooterWidget();
    void getFooterWidget();

    void setModel();

    void moveSections();

    void cleanupTestCase();

};

DECLARE_TEST( TestAdvancedTableView )

#endif // TESTADVANCEDTABLEVIEW_H
