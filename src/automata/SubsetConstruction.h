/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SubsetConstruction.h
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
/**
 * \brief 子集构造算法
 *
 * 将 NFA 通过子集构造法转化为等价的 DFA。
 */
class SubsetConstruction
{
   public:
    /**
     * \brief 构建 DFA
     * \param nfa 源 NFA
     * \return 等价的确定性有限自动机
     */
    static DFA build(const NFA& nfa);
};