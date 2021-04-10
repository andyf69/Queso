#pragma once
#include <QtWidgets/QWidget>
#include "ui_CBankingWidget.h"

class CBankingWidget : public QWidget
{
public:
    explicit CBankingWidget(QWidget* pParent);

private:
    Ui::CBankingWidget ui;
};

