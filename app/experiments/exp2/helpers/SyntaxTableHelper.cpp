/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxTableHelper.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "SyntaxTableHelper.h"

void SyntaxTableHelper::fillFirstTable(QTableWidget*                          tbl,
                                       const QMap<QString, QVector<QString>>& firstRows)
{
    if (!tbl)
        return;
    tbl->clear();
    tbl->setColumnCount(2);
    QStringList hf;
    hf << "非终结符" << "First";
    tbl->setHorizontalHeaderLabels(hf);
    tbl->setRowCount(firstRows.size());
    int r = 0;
    for (auto it = firstRows.begin(); it != firstRows.end(); ++it)
    {
        tbl->setItem(r, 0, new QTableWidgetItem(it.key()));
        tbl->setItem(r, 1, new QTableWidgetItem(it.value().join(", ")));
        r++;
    }
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SyntaxTableHelper::fillFollowTable(QTableWidget*                       tbl,
                                        const QMap<QString, QSet<QString>>& followRows)
{
    if (!tbl)
        return;
    tbl->clear();
    tbl->setColumnCount(2);
    QStringList hf;
    hf << "非终结符" << "Follow";
    tbl->setHorizontalHeaderLabels(hf);
    tbl->setRowCount(followRows.size());
    int r = 0;
    for (auto it = followRows.begin(); it != followRows.end(); ++it)
    {
        tbl->setItem(r, 0, new QTableWidgetItem(it.key()));
        QStringList vals = QStringList(it.value().begin(), it.value().end());
        tbl->setItem(r, 1, new QTableWidgetItem(vals.join(", ")));
        r++;
    }
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void SyntaxTableHelper::syncTokensView(QPlainTextEdit* dest, QPlainTextEdit* source)
{
    if (!dest || !source)
        return;
    dest->setPlainText(source->toPlainText());
}