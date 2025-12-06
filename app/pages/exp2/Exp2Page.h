/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：Exp2Page.h
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
class QLabel;
class QTabWidget;
class QTextEdit;
class QPlainTextEdit;
class QTableWidget;
class QGraphicsView;
class QLineEdit;
class Exp2Page : public QWidget
{
    Q_OBJECT
   public:
    explicit Exp2Page(QWidget* parent = nullptr);
   signals:
    void requestBack();

   private:
    QPushButton* btnBack;
    QTabWidget*  tabSyntax;
    // tabs 封装了控件，保留对象名用于 findChild 查询
};