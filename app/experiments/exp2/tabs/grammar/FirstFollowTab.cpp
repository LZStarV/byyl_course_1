#include "FirstFollowTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

FirstFollowTab::FirstFollowTab(QWidget* parent) : QWidget(parent)
{
    auto l      = new QVBoxLayout(this);
    auto hFF    = new QHBoxLayout;
    tblFirstSet = new QTableWidget;
    tblFirstSet->setObjectName("tblFirstSet");
    tblFollowSet = new QTableWidget;
    tblFollowSet->setObjectName("tblFollowSet");
    hFF->addWidget(tblFirstSet);
    hFF->addWidget(tblFollowSet);
    l->addLayout(hFF);
    // LR(0) 控件已迁移至“LR分析”页签，此处不再包含相关控件
}
