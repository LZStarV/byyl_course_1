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
    QString detail;
    for (const auto& c : r.conflicts)
        detail += QString("[state %1][%2] %3: %4\n")
                      .arg(c.state)
                      .arg(c.terminal)
                      .arg(c.type)
                      .arg(c.detail);
    auto txt = new QPlainTextEdit;
    txt->setReadOnly(true);
    txt->setPlainText(detail);
    v->addWidget(txt);
    resize(700, 500);
}