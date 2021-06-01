#pragma once
#include <QtWidgets/QMainWindow>

class QStackedWidget;

namespace Queso
{
    namespace MainWidget
    {
        enum class Type;
    }

    class CMainWidget;
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

        void onEditAccountList();
        void onEditPayeeList();

        void onAccountActivated(const QModelIndex& index);

        virtual void closeEvent(QCloseEvent* pEvent);

        void saveStateSettings();
        void restoreStateSettings();

        CMainWidget* makeWidget(const MainWidget::Type eWidgetType) const;

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
}
