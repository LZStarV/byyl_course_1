/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SLR.h
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
#include "LL1.h"

struct SLRConflict
{
    int     state;
    QString terminal;
    QString type;
    QString detail;
};

struct SLRCheckResult
{
    bool                 isSLR1 = false;
    QVector<SLRConflict> conflicts;
};

class SLR
{
   public:
    static SLRCheckResult check(const Grammar& g, const LL1Info& ll1);
};