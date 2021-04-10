#pragma once
#include <QtWidgets/QWidget>
#include "ui_CFrontPageWidget.h"

class CFrontPageWidget : public QWidget
{
public:
    explicit CFrontPageWidget(QWidget* pParent);

private:
    Ui::CFrontPageWidget ui;

    void onCreate();
    void onRead();
    void onUpdate();
    void onDelete();
};
