/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：DotService.h
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
class MainWindow;
class NotificationService;

class DotService
{
   public:
    DotService(MainWindow* mw, NotificationService* notify);
    QString ensureGraphDir() const;
    QString pickDotSavePath(const QString& suggestedName) const;
    QString pickImageSavePath(const QString& suggestedName, const QString& fmt) const;
    bool    renderToFile(const QString& dotContent,
                         const QString& outPath,
                         const QString& fmt,
                         int            dpi) const;
    bool    renderToTempPng(const QString& dotContent, QString& outPngPath, int dpi) const;
    void    previewPng(const QString& pngPath, const QString& title) const;

   private:
    MainWindow*          mw_;
    NotificationService* notify_;
};