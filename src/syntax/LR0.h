/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR0.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include <QSet>
#include "Grammar.h"

struct LR0Item
{
    QString          left;
    QVector<QString> right;
    int              dot = 0;
    bool             operator==(const LR0Item& o) const
    {
        return left == o.left && right == o.right && dot == o.dot;
    }
};

struct LR0Graph
{
    QVector<QVector<LR0Item>>     states;
    QMap<int, QMap<QString, int>> edges;
};

class LR0Builder
{
   public:
    static LR0Graph build(const Grammar& g);
    static QString  toDot(const LR0Graph& gr);
};