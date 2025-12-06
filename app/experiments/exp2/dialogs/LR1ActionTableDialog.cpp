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
    auto v    = new QVBoxLayout(this);
    auto lblA = new QLabel(QStringLiteral("ACTION"));
    v->addWidget(lblA);
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
    v->addWidget(tblA);
    auto lblG = new QLabel(QStringLiteral("GOTO"));
    v->addWidget(lblG);
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
    v->addWidget(tblG);
    resize(1000, 700);
}