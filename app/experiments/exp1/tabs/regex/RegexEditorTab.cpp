/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：RegexEditorTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "RegexEditorTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>

RegexEditorTab::RegexEditorTab(QWidget* parent) : QWidget(parent)
{
    auto l        = new QVBoxLayout(this);
    txtInputRegex = new QTextEdit;
    txtInputRegex->setObjectName("txtInputRegex");
    auto h       = new QHBoxLayout;
    btnLoadRegex = new QPushButton("从文件加载");
    btnLoadRegex->setObjectName("btnLoadRegex");
    btnSaveRegex = new QPushButton("保存正则");
    btnSaveRegex->setObjectName("btnSaveRegex");
    btnStartConvert = new QPushButton("转换");
    btnStartConvert->setObjectName("btnStartConvert");
    h->addWidget(btnLoadRegex);
    h->addWidget(btnSaveRegex);
    h->addWidget(btnStartConvert);
    l->addWidget(txtInputRegex);
    l->addLayout(h);
}