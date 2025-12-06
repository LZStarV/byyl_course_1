/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Thompson.h
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
#include "../regex/RegexParser.h"
/**
 * \brief Thompson 构造法
 *
 * 根据正则 AST 直接构造等价的 NFA。
 */
class Thompson
{
   public:
    /**
     * \brief 构建 NFA
     * \param ast 正则表达式 AST 根节点
     * \param alpha 字母表
     * \return 构造得到的 NFA
     */
    static NFA build(ASTNode* ast, Alphabet alpha);
};