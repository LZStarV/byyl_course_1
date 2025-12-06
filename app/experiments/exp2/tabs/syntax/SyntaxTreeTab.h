/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxTreeTab.h
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
class QPlainTextEdit;
class QPushButton;
class QGraphicsView;
class SyntaxTreeTab : public QWidget
{
    Q_OBJECT
   public:
    explicit SyntaxTreeTab(QWidget* parent = nullptr);
    QLabel*         lblSyntaxTreeTitle;
    QPlainTextEdit* txtTokensViewSyntax;
    QPushButton*    btnExportSyntaxDot;
    QPushButton*    btnPreviewSyntaxTree;
    QPushButton*    btnRunSyntaxAnalysis;
    QGraphicsView*  viewSyntaxTree;
};