#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CMainWindow.h"

class CMainWindow : public QMainWindow
{
public:
    CMainWindow();

private:
    void initMenus();
    void initStatusBar();
    void initAccountList();

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
    QAction* m_pImportCsvAction;
    QAction* m_pExitAction;
    QAction* m_pImportQifAction;
    QAction* m_pAccountListAction;
    QMenu* m_pFileMenu;
    QMenu* m_pEditMenu;
    QMenuBar* m_pMenuBar;
    QStatusBar* m_pStatusBar;
    QDockWidget* m_pAccountsDockWidget;
};
