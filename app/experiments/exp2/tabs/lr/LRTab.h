/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LRTab.h
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
class QPushButton;
class QLineEdit;
class LRTab : public QWidget
{
    Q_OBJECT
   public:
    explicit LRTab(QWidget* parent = nullptr);
    QPushButton* btnExportLR0;
    QPushButton* btnPreviewLR0;
    QLineEdit*   edtGraphDpiLR0;
    QPushButton* btnViewLR0Table;
    QPushButton* btnCheckSLR1;
    QPushButton* btnExportLR1;
    QPushButton* btnPreviewLR1;
    QLineEdit*   edtGraphDpiLR1;
    QPushButton* btnViewLR1Table;
    QPushButton* btnViewLR1Action;
};