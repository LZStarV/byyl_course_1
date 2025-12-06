/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：GrammarEditorTab.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QWidget>
class QLabel;
class QTextEdit;
class QPushButton;
class GrammarEditorTab : public QWidget
{
    Q_OBJECT
   public:
    explicit GrammarEditorTab(QWidget* parent = nullptr);
    QLabel*      lblGrammarTitle;
    QTextEdit*   txtInputGrammar;
    QPushButton* btnLoadGrammar;
    QPushButton* btnParseGrammar;
};