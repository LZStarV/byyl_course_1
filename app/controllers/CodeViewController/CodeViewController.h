/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：CodeViewController.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QObject>
class QTabWidget;
class QWidget;
class MainWindow;

class CodeViewController : public QObject
{
    Q_OBJECT
   public:
    explicit CodeViewController(MainWindow* mw);
    void bind(QTabWidget* tabs);
   public slots:
    void onOuterTabChanged(int idx);

   private:
    MainWindow* mw_;
};