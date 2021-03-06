#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CMainWindow.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow();

private:
    void initMenuActions();

    void onImportCSV();
    void onImportQIF();
    void onExit();

    void onCreate();
    void onRead();
    void onUpdate();
    void onDelete();

    Ui::CMainWindowClass ui;
};
