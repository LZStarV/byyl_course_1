/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LRTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "LRTab.h"
#include "../../../../components/ExportGraphButton/ExportGraphButton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

LRTab::LRTab(QWidget* parent) : QWidget(parent)
{
    auto v       = new QVBoxLayout(this);
    auto lr0Box  = new QHBoxLayout;
    btnExportLR0 = new ExportGraphButton("导出LR(0)");
    btnExportLR0->setObjectName("exportBtnLR0");
    lr0Box->addWidget(btnExportLR0);
    btnViewLR0Table = new QPushButton("查看LR(0)表格");
    btnViewLR0Table->setObjectName("btnViewLR0Table");
    lr0Box->addWidget(btnViewLR0Table);
    v->addLayout(lr0Box);

    auto slrBox  = new QHBoxLayout;
    btnCheckSLR1 = new QPushButton("判断SLR(1)文法");
    btnCheckSLR1->setObjectName("btnCheckSLR1");
    slrBox->addWidget(btnCheckSLR1);
    v->addLayout(slrBox);
    auto lr1Box  = new QHBoxLayout;
    btnExportLR1 = new ExportGraphButton("导出LR(1)");
    btnExportLR1->setObjectName("exportBtnLR1");
    btnViewLR1Table = new QPushButton("查看LR(1)表格");
    btnViewLR1Table->setObjectName("btnViewLR1Table");
    btnViewLR1Action = new QPushButton("查看LR(1)分析表");
    btnViewLR1Action->setObjectName("btnViewLR1Action");
    lr1Box->addWidget(btnExportLR1);
    lr1Box->addWidget(btnViewLR1Table);
    lr1Box->addWidget(btnViewLR1Action);
    v->addLayout(lr1Box);
}
