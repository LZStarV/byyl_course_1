/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：CodeViewController.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "CodeViewController.h"
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include "../../../src/config/Config.h"
#include "../../mainwindow.h"

CodeViewController::CodeViewController(MainWindow* mw) : mw_(mw) {}

void CodeViewController::bind(QTabWidget* tabs)
{
    QObject::connect(
        tabs, &QTabWidget::currentChanged, this, &CodeViewController::onOuterTabChanged);
}

void CodeViewController::onOuterTabChanged(int idx)
{
    if (idx < 0)
        return;
    auto     tabs = qobject_cast<QTabWidget*>(sender());
    QWidget* w    = tabs ? tabs->widget(idx) : nullptr;
    if (!w)
        return;
    if (auto codeView = w->findChild<QPlainTextEdit*>("txtGeneratedCode"))
    {
        QString path = mw_->codePath();
        if (!path.isEmpty())
        {
            QFile f(path);
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&f);
                codeView->setPlainText(in.readAll());
                f.close();
            }
        }
    }
    if (auto syntaxCodeView = w->findChild<QPlainTextEdit*>("txtSyntaxGeneratedCode"))
    {
        QString path = Config::generatedOutputDir() + "/syntax/syntax_parser.cpp";
        QFile   f(path);
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&f);
            syntaxCodeView->setPlainText(in.readAll());
            f.close();
        }
    }
}