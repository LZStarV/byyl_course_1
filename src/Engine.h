/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Engine.h
 *           状态表生成、代码生成与运行、以及 LL(1) 语法分析的展示数据生成。
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月7日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QString>
#include <QVector>
#include <QMap>
#include "regex/RegexLexer.h"
#include "regex/RegexParser.h"
#include "automata/Thompson.h"
#include "automata/SubsetConstruction.h"
#include "automata/Hopcroft.h"
#include "generator/CodeGenerator.h"
#include "syntax/Grammar.h"
#include "syntax/LL1.h"
/**
 * \brief 状态表结构
 *
 * 用于在界面展示 NFA/DFA/MinDFA 的转移与标记。
 */
struct Tables
{
    QVector<QString>          columns;
    QVector<QString>          marks;
    QVector<QString>          states;
    QVector<QVector<QString>> rows;
};
/**
 * \brief 词法处理引擎
 *
 * 负责从规则文本到最小化自动机与代码生成的完整流水线：
 * 词法 -> 解析 -> AST -> NFA -> DFA -> MinDFA -> 代码生成与运行。
 */
class Engine
{
   public:
    /**
     * @param {QString} text - 规则文件文本内容
     * @return {RegexFile} 解析后的规则文件（含宏与 Token 头）
     * @note 文本需符合 Token 头与宏规则约定
     */
    RegexFile lexFile(const QString& text);
    /**
     * @param {RegexFile} f - 词法解析得到的规则文件
     * @return {ParsedFile} 包含 AST、Token、Alphabet 的解析结果
     */
    ParsedFile parseFile(const RegexFile& f);
    /**
     * @param {ASTNode*} ast - 正则表达式 AST
     * @param {Alphabet} alpha - 统一字母表
     * @return {NFA} 构建的非确定有限自动机
     */
    NFA buildNFA(ASTNode* ast, const Alphabet& alpha);
    /**
     * @param {NFA} nfa - 非确定自动机
     * @return {DFA} 确定有限自动机
     */
    DFA buildDFA(const NFA& nfa);
    /**
     * @param {DFA} dfa - 确定自动机
     * @return {MinDFA} 最小化自动机
     */
    MinDFA buildMinDFA(const DFA& dfa);
    NFA    buildMergedNFA(const ParsedFile& pf);
    DFA    buildMergedDFA(const ParsedFile& pf);
    MinDFA buildMergedMinDFA(const ParsedFile& pf);
    /**
     * @param {NFA} nfa - 非确定自动机
     * @return {Tables} 包含列、标记、状态与行的表格数据
     */
    Tables nfaTable(const NFA& nfa);
    /**
     * @param {DFA} dfa - 确定自动机
     * @return {Tables} 表格数据
     */
    Tables dfaTable(const DFA& dfa);
    /**
     * @param {MinDFA} dfa - 最小化自动机
     * @return {Tables} 表格数据
     */
    Tables minTable(const MinDFA& dfa);
    /**
     * @param {NFA/DFA/MinDFA} 自动机 - 对应自动机实例
     * @param {QMap<QString,Rule>} macros - 宏规则映射
     * @return {Tables} 表格数据
     */
    Tables nfaTableWithMacros(const NFA& nfa, const QMap<QString, Rule>& macros);
    Tables dfaTableWithMacros(const DFA& dfa, const QMap<QString, Rule>& macros);
    Tables minTableWithMacros(const MinDFA& dfa, const QMap<QString, Rule>& macros);
    /**
     * @param {MinDFA} mdfa - 最小化自动机
     * @param {QMap<QString,int>} tokenCodes - Token 名称到编码的映射
     * @return {QString} C++ 源码文本
     */
    QString generateCode(const MinDFA& mdfa, const QMap<QString, int>& tokenCodes);
    /**
     * @param {MinDFA} mdfa - 最小化自动机
     * @param {QString} source - 源文本
     * @param {int} tokenCode - 目标 Token 编码（接受终止）
     * @return {QString} 编码结果（含 ERR 标记）
     */
    QString run(const MinDFA& mdfa, const QString& source, int tokenCode);
    /**
     * @param {ParsedFile} pf - 解析结果
     * @param {QVector<int>&} codes - 输出：Token 编码顺序
     * @return {QVector<MinDFA>} 多个最小化自动机
     */
    QVector<MinDFA> buildAllMinDFA(const ParsedFile& pf, QVector<int>& codes);
    /**
     * @param {QVector<MinDFA>} mdfas - 多个最小化自动机
     * @param {QVector<int>} codes - 对应编码序列
     * @param {QString} source - 源文本
     * @param {QSet<int>} identifierCodes - 标识符编码集合（用于词素处理）
     * @return {QString} 编码序列（以空格分隔）
     */
    QString runMultiple(const QVector<MinDFA>& mdfas,
                        const QVector<int>&    codes,
                        const QString&         source,
                        const QSet<int>&       identifierCodes);
    /**
     * @param {QString} text - 文法文本（BNF）
     * @param {QString&} error - 输出：错误信息
     * @return {Grammar} 解析结果
     */
    Grammar                               parseGrammarText(const QString& text, QString& error);
    LL1Info                               computeLL1(const Grammar& g);
    QMap<QString, QVector<QString>>       firstFollowAsRows(const LL1Info& info);
    QMap<QString, QVector<QString>>       firstAsRows(const Grammar& g, const LL1Info& info);
    QMap<QString, QMap<QString, QString>> parsingTableAsRows(const Grammar& g, const LL1Info& info);
};
