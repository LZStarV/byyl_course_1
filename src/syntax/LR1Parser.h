/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1Parser.h
 *           支持步骤记录与语义 AST 构建。
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
#include "Grammar.h"
#include "LR1.h"
#include "AST.h"

struct ParseTreeNode
{
    QString                 symbol;
    QVector<ParseTreeNode*> children;
};

struct ParseStep
{
    int                          step = 0;
    QVector<QPair<int, QString>> stack;
    QVector<QString>             rest;
    QString                      action;
    QString                      production;
};

struct ParseResult
{
    QVector<ParseStep> steps;
    ParseTreeNode*     root     = nullptr;
    int                errorPos = -1;
    SemanticASTNode*   astRoot  = nullptr;
    QVector<ParseStep> semanticSteps;
};

class LR1Parser
{
   public:
    /**
     * @param {QVector<QString>} tokens - 终结符名称序列，末尾隐含 `$`
     * @param {Grammar} g - 文法
     * @param {LR1ActionTable} t - LR(1) 动作/GOTO 表
     * @return {ParseResult} 包含步骤与解析树的结果
     */
    static ParseResult parse(const QVector<QString>& tokens,
                             const Grammar&          g,
                             const LR1ActionTable&   t);
    /**
     */
    static ParseResult parseWithSemantics(const QVector<QString>&                     tokens,
                                          const Grammar&                              g,
                                          const LR1ActionTable&                       t,
                                          const QMap<QString, QVector<QVector<int>>>& actions,
                                          const QMap<int, QString>&                   roleMeaning,
                                          const QString&                              rootPolicy,
                                          const QString&                              childOrder);
    /**
     */
    static ParseResult parseWithSemantics(const QVector<QString>&                     tokens,
                                          const Grammar&                              g,
                                          const LR1ActionTable&                       t,
                                          const QMap<QString, QVector<QVector<int>>>& actions,
                                          const QMap<int, QString>&                   roleMeaning,
                                          const QString&                              rootPolicy,
                                          const QString&                              childOrder,
                                          const QVector<QString>&                     lexemes);
};
