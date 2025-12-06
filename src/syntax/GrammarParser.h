/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：GrammarParser.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include "Grammar.h"
#include <QString>

namespace GrammarParser
{
    Grammar parseFile(const QString& path, QString& error);
    Grammar parseString(const QString& text, QString& error);
}  // namespace GrammarParser