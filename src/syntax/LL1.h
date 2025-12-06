/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LL1.h
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
#include <QSet>
#include <QMap>
#include <QVector>
#include "Grammar.h"

struct LL1Info
{
    QMap<QString, QSet<QString>>      first;
    QMap<QString, QSet<QString>>      follow;
    QMap<QString, QMap<QString, int>> table;
    QVector<QString>                  conflicts;
};

class LL1
{
   public:
    static LL1Info compute(const Grammar& g);
};