/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：FirstFollowTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
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