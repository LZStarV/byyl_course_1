/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxCodeViewTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "SyntaxCodeViewTab.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>

SyntaxCodeViewTab::SyntaxCodeViewTab(QWidget* parent) : QWidget(parent)
{
    auto l             = new QVBoxLayout(this);
    lblSyntaxCodeTitle = new QLabel("语法分析器代码查看");
    l->addWidget(lblSyntaxCodeTitle);
    txtSyntaxGeneratedCode = new QPlainTextEdit;
    txtSyntaxGeneratedCode->setObjectName("txtSyntaxGeneratedCode");
    txtSyntaxGeneratedCode->setReadOnly(true);
    l->addWidget(txtSyntaxGeneratedCode);
}