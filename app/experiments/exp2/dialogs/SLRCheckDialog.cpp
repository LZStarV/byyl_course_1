/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SLRCheckDialog.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "SLRCheckDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>

SLRCheckDialog::SLRCheckDialog(const SLRCheckResult& r, QWidget* parent) : QDialog(parent)
{
    setWindowTitle(QStringLiteral("SLR(1) 判断结果"));
    auto v   = new QVBoxLayout(this);
    auto lbl = new QLabel(r.isSLR1 ? QStringLiteral("该文法为 SLR(1) 文法")
                                   : QStringLiteral("该文法不是 SLR(1) 文法"));
    v->addWidget(lbl);
    auto txt = new QPlainTextEdit;
    txt->setReadOnly(true);
    QString out;
    auto    typeCN = [](const QString& t)
    {
        if (t == "shift/reduce")
            return QStringLiteral("移进/规约冲突");
        if (t == "reduce/reduce")
            return QStringLiteral("规约/规约冲突");
        return QStringLiteral("冲突");
    };
    auto actionCN = [](const QString& a)
    {
        if (a.startsWith("s"))
        {
            bool ok = false;
            int  st = a.mid(1).toInt(&ok);
            return ok ? QStringLiteral("移进到状态 %1").arg(st) : a;
        }
        if (a.startsWith("r "))
        {
            // r A -> α
            return QStringLiteral("规约 %1").arg(a.mid(2));
        }
        return a;
    };
    for (const auto& c : r.conflicts)
    {
        QStringList acts = c.detail.split('|');
        QStringList cnActs;
        for (const auto& a : acts) cnActs << actionCN(a);
        out += QStringLiteral("[状态 %1][输入 %2] %3：%4\n")
                   .arg(c.state)
                   .arg(c.terminal)
                   .arg(typeCN(c.type))
                   .arg(cnActs.join(" | "));
    }
    txt->setPlainText(out);
    v->addWidget(txt);
    resize(700, 500);
}
