/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：RegexController.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "RegexController.h"
#include <QWidget>
#include <QPushButton>
#include "../../mainwindow.h"

RegexController::RegexController(MainWindow* mw) : mw_(mw) {}

void RegexController::bind(QWidget* regexPage)
{
    auto btnLoad    = regexPage->findChild<QPushButton*>("btnLoadRegex");
    auto btnSave    = regexPage->findChild<QPushButton*>("btnSaveRegex");
    auto btnConvert = regexPage->findChild<QPushButton*>("btnStartConvert");
    if (btnLoad)
        QObject::connect(btnLoad, &QPushButton::clicked, mw_, &MainWindow::loadRegex);
    if (btnSave)
        QObject::connect(btnSave, &QPushButton::clicked, mw_, &MainWindow::saveRegex);
    if (btnConvert)
        QObject::connect(btnConvert, &QPushButton::clicked, mw_, &MainWindow::startConvert);
}