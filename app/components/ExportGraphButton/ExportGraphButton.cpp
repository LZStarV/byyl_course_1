/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：ExportGraphButton.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "ExportGraphButton.h"
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include "../../services/DotService/DotService.h"
#include "../ToastManager/ToastManager.h"

ExportGraphButton::ExportGraphButton(const QString& text, QWidget* parent) :
    QPushButton(text, parent)
{
    setupMenu();
}

void ExportGraphButton::setupMenu()
{
    menu_       = new QMenu(this);
    auto actDot = menu_->addAction(QStringLiteral("导出DOT"));
    auto actImg = menu_->addAction(QStringLiteral("导出图片"));
    setMenu(menu_);
    connect(actDot, &QAction::triggered, this, &ExportGraphButton::exportDot);
    connect(actImg, &QAction::triggered, this, &ExportGraphButton::exportImage);
}

void ExportGraphButton::setDotService(DotService* svc)
{
    svc_ = svc;
}

void ExportGraphButton::setDotSupplier(std::function<QString()> supplier)
{
    supplier_ = std::move(supplier);
}

void ExportGraphButton::setSuggestedBasename(const QString& base)
{
    base_ = base;
}

void ExportGraphButton::setDpiProvider(std::function<int()> provider)
{
    dpiProvider_ = std::move(provider);
}

void ExportGraphButton::exportDot()
{
    if (!svc_ || !supplier_)
        return;
    QString dot = supplier_();
    if (dot.trimmed().isEmpty())
    {
        ToastManager::instance().showWarning(QStringLiteral("没有可导出的内容"));
        return;
    }
    QString ts   = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString def  = base_ + ts + ".dot";
    QString path = svc_->pickDotSavePath(def);
    if (path.isEmpty())
        return;
    QFile f(path);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream o(&f);
        o << dot;
        f.close();
    }
}

void ExportGraphButton::exportImage()
{
    if (!svc_ || !supplier_)
        return;
    int     dpi = dpiProvider_ ? dpiProvider_() : 150;
    QString dot = supplier_();
    if (dot.trimmed().isEmpty())
    {
        ToastManager::instance().showWarning(QStringLiteral("没有可导出的内容"));
        return;
    }
    QString ts   = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
    QString def  = base_ + ts + ".png";
    QString path = svc_->pickImageSavePath(def, "png");
    if (path.isEmpty())
        return;
    svc_->renderToFile(dot, path, "png", dpi);
}