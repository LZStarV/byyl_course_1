/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：NotificationService.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QString>
class QMainWindow;

class NotificationService
{
   public:
    explicit NotificationService(QMainWindow* mw = nullptr);
    void setMainWindow(QMainWindow* mw);
    void info(const QString& text);
    void warning(const QString& text);
    void error(const QString& text);

   private:
    QMainWindow* mw_;
};