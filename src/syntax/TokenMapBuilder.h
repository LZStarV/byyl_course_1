/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：TokenMapBuilder.h
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
#include <QMap>
#include "../regex/RegexParser.h"

class TokenMapBuilder
{
   public:
    static QMap<QString, QString> build(const QString& regexText, const ParsedFile& pf);
    static bool                   saveJson(const QMap<QString, QString>& m, const QString& path);
};