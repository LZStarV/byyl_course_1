/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：DotExporter.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include "../model/Automata.h"
#include "../regex/RegexLexer.h"
#include <QString>

class DotExporter
{
   public:
    static QString toDot(const NFA& nfa);
    static QString toDot(const DFA& dfa);
    static QString toDot(const MinDFA& mdfa);
    static QString toDot(const NFA& nfa, const QMap<QString, Rule>& macros);
    static QString toDot(const DFA& dfa, const QMap<QString, Rule>& macros);
    static QString toDot(const MinDFA& mdfa, const QMap<QString, Rule>& macros);
    static bool    exportToDot(const NFA& nfa, const QString& filename);
    static bool    exportToDot(const DFA& dfa, const QString& filename);
    static bool    exportToDot(const MinDFA& mdfa, const QString& filename);
    static bool    exportToDot(const NFA&                 nfa,
                               const QMap<QString, Rule>& macros,
                               const QString&             filename);
    static bool    exportToDot(const DFA&                 dfa,
                               const QMap<QString, Rule>& macros,
                               const QString&             filename);
    static bool    exportToDot(const MinDFA&              mdfa,
                               const QMap<QString, Rule>& macros,
                               const QString&             filename);
};