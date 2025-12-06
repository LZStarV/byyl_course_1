#include "AutomataTableHelper.h"

void AutomataTableHelper::fillTable(QTableWidget* tbl, const Tables& t)
{
    if (!tbl)
        return;
    tbl->clear();
    tbl->setColumnCount(t.columns.size());
    tbl->setRowCount(t.rows.size());
    QStringList headers;
    for (auto c : t.columns) headers << c;
    tbl->setHorizontalHeaderLabels(headers);
    for (int r = 0; r < t.rows.size(); ++r)
    {
        auto row = t.rows[r];
        for (int c = 0; c < row.size(); ++c)
        {
            tbl->setItem(r, c, new QTableWidgetItem(row[c]));
        }
    }
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pruneEmptyColumns(tbl);
}

QVector<QString> AutomataTableHelper::unionSyms(const QVector<Tables>& tables, bool includeEps)
{
    QSet<QString> s;
    for (const auto& t : tables)
    {
        for (int i = 2; i < t.columns.size(); ++i)
        {
            QString c = t.columns[i];
            if (includeEps || c != "#")
                s.insert(c);
        }
    }
    QVector<QString> v = QVector<QString>(s.begin(), s.end());
    std::sort(v.begin(), v.end());
    return v;
}

void AutomataTableHelper::pruneEmptyColumns(QTableWidget* tbl)
{
    if (!tbl) return;
    QSet<QString> keep;
    keep.insert(QStringLiteral("标记"));
    keep.insert(QStringLiteral("状态 ID"));
    keep.insert(QStringLiteral("状态集合"));
    for (int c = tbl->columnCount() - 1; c >= 0; --c)
    {
        QString header = tbl->horizontalHeaderItem(c) ? tbl->horizontalHeaderItem(c)->text() : QString();
        if (keep.contains(header)) continue;
        bool any = false;
        for (int r = 0; r < tbl->rowCount(); ++r)
        {
            auto it = tbl->item(r, c);
            if (it && !it->text().trimmed().isEmpty()) { any = true; break; }
        }
        if (!any) tbl->removeColumn(c);
    }
}
