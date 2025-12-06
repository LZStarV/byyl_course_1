/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1ActionTableDialog.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "LR1ActionTableDialog.h"
#include <QVBoxLayout>
#include <QLabel>

LR1ActionTableDialog::LR1ActionTableDialog(const Grammar&        g,
                                           const LR1ActionTable& t,
                                           QWidget*              parent) :
    QDialog(parent)
{
    setWindowTitle(QStringLiteral("LR(1) 分析表"));
    auto h     = new QHBoxLayout(this);
    auto left  = new QVBoxLayout;
    auto right = new QVBoxLayout;

    auto lblA = new QLabel(QStringLiteral("ACTION"));
    left->addWidget(lblA);
    QStringList termCols;
    termCols << "state";
    for (const auto& s : g.terminals)
        if (s != "#")
            termCols << s;
    termCols << "$";
    auto tblA = new QTableWidget;
    tblA->setColumnCount(termCols.size());
    tblA->setHorizontalHeaderLabels(termCols);
    tblA->setRowCount(t.action.size());
    int r = 0;
    for (auto sit = t.action.begin(); sit != t.action.end(); ++sit)
    {
        int st = sit.key();
        tblA->setItem(r, 0, new QTableWidgetItem(QString::number(st)));
        for (int c = 1; c < termCols.size(); ++c)
        {
            QString a   = termCols[c];
            QString val = sit.value().value(a);
            tblA->setItem(r, c, new QTableWidgetItem(val));
        }
        ++r;
    }
    tblA->setEditTriggers(QAbstractItemView::NoEditTriggers);
    left->addWidget(tblA);

    auto lblG = new QLabel(QStringLiteral("GOTO"));
    left->addWidget(lblG);
    QStringList ntCols;
    ntCols << "state";
    for (const auto& s : g.nonterminals) ntCols << s;
    auto tblG = new QTableWidget;
    tblG->setColumnCount(ntCols.size());
    tblG->setHorizontalHeaderLabels(ntCols);
    tblG->setRowCount(t.gotoTable.size());
    r = 0;
    for (auto sit = t.gotoTable.begin(); sit != t.gotoTable.end(); ++sit)
    {
        int st = sit.key();
        tblG->setItem(r, 0, new QTableWidgetItem(QString::number(st)));
        for (int c = 1; c < ntCols.size(); ++c)
        {
            QString A  = ntCols[c];
            int     to = sit.value().value(A, -1);
            tblG->setItem(r, c, new QTableWidgetItem(to >= 0 ? QString::number(to) : QString()));
        }
        ++r;
    }
    tblG->setEditTriggers(QAbstractItemView::NoEditTriggers);
    left->addWidget(tblG);

    auto lblR = new QLabel(QStringLiteral("规约规则"));
    right->addWidget(lblR);
    auto tblR = new QTableWidget;
    tblR->setColumnCount(2);
    tblR->setHorizontalHeaderLabels(QStringList()
                                    << QStringLiteral("规约编号") << QStringLiteral("规约规则"));
    tblR->setRowCount(t.reductions.size());
    for (int i = 0; i < t.reductions.size(); ++i)
    {
        const auto& pr = t.reductions[i];
        tblR->setItem(i, 0, new QTableWidgetItem(QString("r%1").arg(pr.first)));
        tblR->setItem(i, 1, new QTableWidgetItem(pr.second));
    }
    tblR->setEditTriggers(QAbstractItemView::NoEditTriggers);
    right->addWidget(tblR);

    h->addLayout(left);
    h->addLayout(right);
    h->setStretch(0, 4);
    h->setStretch(1, 1);
    resize(1100, 700);
}
