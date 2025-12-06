/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Grammar.h
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

struct Production
{
    QString          left;
    QVector<QString> right;
    int              line = -1;
};

class Grammar
{
   public:
    QSet<QString>                      terminals;
    QSet<QString>                      nonterminals;
    QString                            startSymbol;
    QMap<QString, QVector<Production>> productions;
    bool                               hasEpsilon(const QVector<QString>& rhs) const;
};