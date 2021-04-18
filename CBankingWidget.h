#pragma once
#include <QtWidgets/QWidget>
#include "ui_CBankingWidget.h"

class CBankingWidget : public QWidget
{
public:
    explicit CBankingWidget(QWidget* pParent);

    void setAccount(const int iAccountId);

private:
    Ui::CBankingWidget ui;
};

