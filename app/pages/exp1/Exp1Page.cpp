/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Exp1Page.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "Exp1Page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

Exp1Page::Exp1Page(QWidget* parent) : QWidget(parent)
{
    vroot    = new QVBoxLayout(this);
    auto bar = new QHBoxLayout;
    btnBack  = new QPushButton("← 返回");
    bar->addWidget(btnBack);
    bar->addStretch(1);
    vroot->addLayout(bar);
    content = new QWidget;
    auto v  = new QVBoxLayout(content);
    v->setContentsMargins(0, 0, 0, 0);
    vroot->addWidget(content);
    connect(btnBack, &QPushButton::clicked, this, [this]() { emit requestBack(); });
}

QWidget* Exp1Page::contentWidget() const
{
    return content;
}