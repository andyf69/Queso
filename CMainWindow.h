#pragma once

#include <QtWidgets/QMainWindow>

class QStackedWidget;
class CMainWindow : public QMainWindow
{
public:
    CMainWindow();

private:
    enum class WidgetType
    {
        FrontPage,
        Banking,
        Investment,
        Retirement,
        Asset,
        Liability
    };

private:
    void initMenus();
    void initStatusBar();
    void initAccountList();

    void onImportCSV();
    void onImportQIF();
    void onExit();

    void onEditAccountList();
    void onEditPayeeList();

    void onAccountActivated(const QModelIndex& index);

    virtual void closeEvent(QCloseEvent* pEvent);

    void saveStateSettings();
    void restoreStateSettings();

    QWidget* makeWidget(const WidgetType eWidgetType) const;

    QAction* m_pImportCsvAction;
    QAction* m_pExitAction;
    QAction* m_pImportQifAction;
    QAction* m_pEditAccountListAction;
    QAction* m_pEditPayeeListAction;
    QMenu* m_pFileMenu;
    QMenu* m_pEditMenu;
    QMenu* m_pViewMenu;
    QMenuBar* m_pMenuBar;
    QStackedWidget* m_pStackedWidget;
    QStatusBar* m_pStatusBar;
    QDockWidget* m_pAccountsDockWidget;
};
