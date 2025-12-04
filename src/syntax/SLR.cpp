#include "SLR.h"
#include "LR0.h"

static bool isTerminal(const QSet<QString>& terms, const QString& s)
{
    return terms.contains(s);
}

SLRCheckResult SLR::check(const Grammar& g, const LL1Info& ll1)
{
    auto                              gr = LR0Builder::build(g);
    QMap<int, QMap<QString, QString>> action;
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
                        QString prev = action[st].value(a);
                        QString now  = QString("s%1").arg(to);
                        if (!prev.isEmpty() && prev != now)
                            action[st][a] = prev + "|" + now;
                        else
                            action[st][a] = now;
                    }
                }
            }
            else
            {
                auto followA = ll1.follow.value(it.left);
                for (const auto& a : followA)
                {
                    QString prev = action[st].value(a);
                    QString now  = QString("r %1 -> %2").arg(it.left).arg(it.right.join(" "));
                    if (!prev.isEmpty() && prev != now)
                        action[st][a] = prev + "|" + now;
                    else
                        action[st][a] = now;
                }
            }
        }
    }
    SLRCheckResult res;
    for (auto sit = action.begin(); sit != action.end(); ++sit)
    {
        int st = sit.key();
        for (auto ait = sit.value().begin(); ait != sit.value().end(); ++ait)
        {
            QString a = ait.key();
            QString v = ait.value();
            if (v.contains("|"))
            {
                SLRConflict c;
                c.state    = st;
                c.terminal = a;
                c.type     = "conflict";
                c.detail   = v;
                res.conflicts.push_back(c);
            }
        }
    }
    res.isSLR1 = res.conflicts.isEmpty();
    return res;
}
