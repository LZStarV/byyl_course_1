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
