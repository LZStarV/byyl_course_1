/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SLR.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "SLR.h"
#include "LR0.h"

static bool isTerminal(const QSet<QString>& terms, const QString& s)
{
    return terms.contains(s);
}

SLRCheckResult SLR::check(const Grammar& g, const LL1Info& ll1)
{
    auto                                    gr = LR0Builder::build(g);
    QMap<int, QMap<QString, QSet<QString>>> actionsSet;
    for (int st = 0; st < gr.states.size(); ++st)
    {
        const auto& items = gr.states[st];
        for (const auto& it : items)
        {
            if (it.dot < it.right.size())
            {
                QString a = it.right[it.dot];
                if (isTerminal(g.terminals, a))
                {
                    int to = gr.edges.value(st).value(a, -1);
                    if (to >= 0)
                    {
                        actionsSet[st][a].insert(QString("s%1").arg(to));
                    }
                }
            }
            else
            {
                auto    followA = ll1.follow.value(it.left);
                QString rhsText = it.right.isEmpty() ? QString("#") : it.right.join(" ");
                QString red     = QString("r %1 -> %2").arg(it.left).arg(rhsText);
                for (const auto& a : followA)
                {
                    actionsSet[st][a].insert(red);
                }
            }
        }
    }
    SLRCheckResult res;
    for (auto sit = actionsSet.begin(); sit != actionsSet.end(); ++sit)
    {
        int st = sit.key();
        for (auto ait = sit.value().begin(); ait != sit.value().end(); ++ait)
        {
            const QString& a   = ait.key();
            const auto&    set = ait.value();
            if (set.size() >= 2)
            {
                bool hasShift = false, hasReduce = false;
                for (const auto& act : set)
                {
                    if (act.startsWith("s"))
                        hasShift = true;
                    if (act.startsWith("r"))
                        hasReduce = true;
                }
                QStringList list = QStringList(set.begin(), set.end());
                std::sort(list.begin(), list.end());
                SLRConflict c;
                c.state    = st;
                c.terminal = a;
                c.type     = (hasShift && hasReduce) ? "shift/reduce" : "reduce/reduce";
                c.detail   = list.join("|");
                res.conflicts.push_back(c);
            }
        }
    }
    res.isSLR1 = res.conflicts.isEmpty();
    return res;
}
