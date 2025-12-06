/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1ProcessTab.h
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
#include <QPushButton>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QTableWidget>

class LR1ProcessTab : public QWidget
{
    Q_OBJECT
   public:
    explicit LR1ProcessTab(QWidget* parent = nullptr);
    QPushButton*    btnLoadDefault;
    QComboBox*      cmbPickSource;
    QPushButton*    btnRunLR1;
    QPlainTextEdit* txtSourceView;
    QPlainTextEdit* txtTokensView;
    QPlainTextEdit* txtGrammarView;
};