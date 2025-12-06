/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：NotificationService.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "NotificationService.h"
#include <QMainWindow>
#include <QStatusBar>
#include "../../components/ToastManager/ToastManager.h"

NotificationService::NotificationService(QMainWindow* mw) : mw_(mw) {}

void NotificationService::setMainWindow(QMainWindow* mw)
{
    mw_ = mw;
}

void NotificationService::info(const QString& text)
{
    if (mw_ && mw_->statusBar())
        mw_->statusBar()->showMessage(text);
    ToastManager::instance().showInfo(text);
}

void NotificationService::warning(const QString& text)
{
    if (mw_ && mw_->statusBar())
        mw_->statusBar()->showMessage(text);
    ToastManager::instance().showWarning(text);
}

void NotificationService::error(const QString& text)
{
    if (mw_ && mw_->statusBar())
        mw_->statusBar()->showMessage(text);
    ToastManager::instance().showError(text);
}