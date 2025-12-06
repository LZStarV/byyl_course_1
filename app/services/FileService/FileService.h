/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：FileService.h
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
class QWidget;

class FileService
{
   public:
    static QString openFile(QWidget* parent, const QString& title, const QString& filter);
    static QString saveFile(QWidget*       parent,
                            const QString& title,
                            const QString& suggested,
                            const QString& filter);
    static bool    readAllText(const QString& path, QString& out);
    static bool    writeAllText(const QString& path, const QString& content);
    static QString ensureDir(const QString& dir);
};