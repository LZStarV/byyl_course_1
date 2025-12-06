/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Hopcroft.h
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
 * \brief Hopcroft 最小化算法
 *
 * 对 DFA 进行状态划分与合并，得到最小化的 MinDFA。
 */
class Hopcroft
{
   public:
    /**
     * \brief 最小化 DFA
     * \param dfa 输入 DFA
     * \return 最小化后的 MinDFA
     */
    static MinDFA minimize(const DFA& dfa);
};