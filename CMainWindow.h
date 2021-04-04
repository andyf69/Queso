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
    void initAccountList(QTreeView* pTreeView);

    void onImportCSV();
    void onImportQIF();
    void onExit();

    void onAccountList();

    void onCreate();
    void onRead();
    void onUpdate();
    void onDelete();

    void onAccountActivated(const QModelIndex& index);

    virtual void closeEvent(QCloseEvent* pEvent);

    void saveStateSettings();
    void restoreStateSettings();

    Ui::CMainWindowClass ui;
};
