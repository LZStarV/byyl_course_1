/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Grammar.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "Grammar.h"

bool Grammar::hasEpsilon(const QVector<QString>& rhs) const
{
    return rhs.size() == 1 && rhs[0] == "#";
}