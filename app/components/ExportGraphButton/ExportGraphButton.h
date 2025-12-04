#pragma once
#include <QPushButton>
#include <functional>
class QMenu;
class DotService;

class ExportGraphButton : public QPushButton
{
    Q_OBJECT
   public:
    explicit ExportGraphButton(const QString& text = QString(), QWidget* parent = nullptr);
    void setDotService(DotService* svc);
    void setDotSupplier(std::function<QString()> supplier);
    void setSuggestedBasename(const QString& base);
    void setDpiProvider(std::function<int()> provider);

   private:
    void                     setupMenu();
    void                     exportDot();
    void                     exportImage();
    QMenu*                   menu_ = nullptr;
    DotService*              svc_  = nullptr;
    std::function<QString()> supplier_;
    std::function<int()>     dpiProvider_;
    QString                  base_ = "graph_";
};
