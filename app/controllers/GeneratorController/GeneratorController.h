/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：GeneratorController.h
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
class QWidget;
class MainWindow;
class Engine;
class NotificationService;

class GeneratorController : public QObject
{
    Q_OBJECT
   public:
    explicit GeneratorController(MainWindow* mw, Engine* engine, NotificationService* notify);
    void bind(QWidget* regexTab, QWidget* codeViewTab);
    void convert();
    void generateCode();
    void compileAndRun();

   private:
    MainWindow*          mw_;
    Engine*              engine_;
    NotificationService* notify_;
    QWidget*             regexTab_ = nullptr;
    QWidget*             codeTab_  = nullptr;
};