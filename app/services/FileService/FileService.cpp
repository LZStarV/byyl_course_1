/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：FileService.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "FileService.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>

QString FileService::openFile(QWidget* parent, const QString& title, const QString& filter)
{
    return QFileDialog::getOpenFileName(parent, title, QString(), filter);
}

QString FileService::saveFile(QWidget*       parent,
                              const QString& title,
                              const QString& suggested,
                              const QString& filter)
{
    return QFileDialog::getSaveFileName(parent, title, suggested, filter);
}

bool FileService::readAllText(const QString& path, QString& out)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&f);
    out = in.readAll();
    f.close();
    return true;
}

bool FileService::writeAllText(const QString& path, const QString& content)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream o(&f);
    o << content;
    f.close();
    return true;
}

QString FileService::ensureDir(const QString& dir)
{
    QDir d(dir);
    if (!d.exists())
        d.mkpath(".");
    return d.absolutePath();
}