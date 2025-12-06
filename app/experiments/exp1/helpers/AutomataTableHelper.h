/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：AutomataTableHelper.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QVector>
#include <QString>
#include <QTableWidget>
#include "../../../../src/Engine.h"

class AutomataTableHelper
{
   public:
    static void                       fillTable(QTableWidget* tbl, const Tables& t);
    static QVector<QString>           unionSyms(const QVector<Tables>& tables, bool includeEps);
    static void                       pruneEmptyColumns(QTableWidget* tbl);
    static QMap<QString, QSet<QChar>> buildMacroSets(const QMap<QString, Rule>& macros);
    static QMap<QString, QString>     buildMacroExprs(const QMap<QString, Rule>& macros);
    static void                       aggregateByMacros(Tables&                           t,
                                                        const QMap<QString, QSet<QChar>>& macroSets,
                                                        const QMap<QString, QString>&     macroExprs);
};