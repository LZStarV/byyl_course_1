/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1TreeTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "LR1TreeTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidget>

LR1TreeTab::LR1TreeTab(QWidget* parent) : QWidget(parent)
{
    auto hMain = new QHBoxLayout(this);
    // 左侧：语法树
    auto left    = new QVBoxLayout;
    auto lblTree = new QLabel(QStringLiteral("语法树"));
    left->addWidget(lblTree);
    auto tree = new QTreeWidget;
    tree->setObjectName("treeSemanticLR1");
    tree->setHeaderHidden(true);
    left->addWidget(tree);
    // 右侧：顶部按钮 + 两个过程表并排
    auto right            = new QVBoxLayout;
    auto btnBar           = new QHBoxLayout;
    btnShowGrammarProcess = new QPushButton(QStringLiteral("查看语法分析过程"));
    btnShowGrammarProcess->setObjectName("btnShowGrammarProcess");
    btnBar->addWidget(btnShowGrammarProcess);
    right->addLayout(btnBar);
    auto rightTables = new QHBoxLayout;
    auto col2        = new QVBoxLayout;
    auto lblProc2    = new QLabel(QStringLiteral("语义分析过程"));
    col2->addWidget(lblProc2);
    tblSemantic = new QTableWidget;
    tblSemantic->setObjectName("tblSemanticProcess");
    tblSemantic->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblSemantic->setSelectionBehavior(QAbstractItemView::SelectRows);
    col2->addWidget(tblSemantic);
    rightTables->addLayout(col2);
    rightTables->setStretch(0, 1);
    right->addLayout(rightTables);
    // 组装主布局
    hMain->addLayout(left);
    hMain->addLayout(right);
    hMain->setStretch(0, 1);
    hMain->setStretch(1, 3);
}
