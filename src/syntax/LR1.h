#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include "Grammar.h"

struct LR1Item
{
    QString          left;
    QVector<QString> right;
    int              dot = 0;
    QString          lookahead;
    bool             operator==(const LR1Item& o) const
    {
        return left == o.left && right == o.right && dot == o.dot && lookahead == o.lookahead;
    }
};

struct LR1Graph
{
    QVector<QVector<LR1Item>>     states;
    QMap<int, QMap<QString, int>> edges;
};

struct LR1ActionTable
{
    QMap<int, QMap<QString, QString>> action;
    QMap<int, QMap<QString, int>>     gotoTable;
};

class LR1Builder
{
   public:
    static LR1Graph       build(const Grammar& g);
    static QString        toDot(const LR1Graph& gr);
    static LR1ActionTable computeActionTable(const Grammar& g, const LR1Graph& gr);
};
