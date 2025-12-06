/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxParser.h
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
#include "Grammar.h"
#include "LL1.h"
#include "AST.h"

struct SyntaxResult
{
    SyntaxASTNode* root     = nullptr;
    int            errorPos = -1;
};

SyntaxResult parseTokens(const QVector<QString>& tokens, const Grammar& g, const LL1Info& info);