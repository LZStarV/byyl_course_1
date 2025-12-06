/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Alphabet.h
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
#include <QVector>
struct Alphabet
{
    QSet<QString> symbols;
    bool          hasLetter               = false;
    bool          hasDigit                = false;
    bool          allowUnderscoreInLetter = false;
    bool          allowDollarInLetter     = false;
    void          add(const QString& s)
    {
        symbols.insert(s);
    }
    QVector<QString> ordered() const
    {
        return QVector<QString>(symbols.begin(), symbols.end());
    }
};