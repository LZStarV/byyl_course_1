/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：TestValidationTab.h
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
class QPlainTextEdit;
class QPushButton;

class TestValidationTab : public QWidget
{
    Q_OBJECT
   public:
    explicit TestValidationTab(QWidget* parent = nullptr);
    QPlainTextEdit* txtSourceTiny;
    QPlainTextEdit* txtLexResult;
    QPushButton*    btnPickSample;
    QPushButton*    btnRunLexer;
    QPushButton*    btnSaveLexResultAs;
};