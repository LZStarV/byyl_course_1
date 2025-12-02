#include "Exp2Page.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

Exp2Page::Exp2Page(QWidget* parent) : QWidget(parent)
{
    auto v = new QVBoxLayout(this);
    auto bar = new QHBoxLayout;
    btnBack = new QPushButton("← 返回");
    bar->addWidget(btnBack);
    bar->addStretch(1);
    v->addLayout(bar);
    lblDev = new QLabel("正在开发中……");
    lblDev->setAlignment(Qt::AlignCenter);
    v->addStretch(1);
    v->addWidget(lblDev);
    v->addStretch(2);
    connect(btnBack, &QPushButton::clicked, this, [this]() { emit requestBack(); });
}
