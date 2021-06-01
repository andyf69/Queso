#include "CMainWindow.h"
#include "CAccount.h"
#include "CAccountListDelegate.h"
#include "CAccountListEditorDlg.h"
#include "CAccountListModel.h"
#include "CBankingWidget.h"
#include "CFinancialInstitution.h"
#include "CFrontPageWidget.h"
#include "CImportCSV.h"
#include "CImportQIF.h"
#include "CPayeeListDlg.h"
#include "CSettings.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlRelationalTableModel>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>


Queso::CMainWindow::CMainWindow()
    : QMainWindow()
{
    setObjectName("CMainWindow");
    setWindowTitle("Queso");
    resize(1200, 800);

    m_pStackedWidget = new QStackedWidget(this);
    m_pStackedWidget->setObjectName("CStackedWidget");

    setCentralWidget(m_pStackedWidget);
    m_pStackedWidget->addWidget(makeWidget(MainWidget::Type::FrontPage));

    initAccountList();
    initMenus();
    initStatusBar();

    restoreStateSettings();
}

void Queso::CMainWindow::initMenus()
{
    m_pImportCsvAction = new QAction("Import CSV...", this);
    m_pImportQifAction = new QAction("Import QIF...", this);
    m_pExitAction = new QAction("Exit", this);
    m_pEditAccountListAction = new QAction("Account List...", this);
    m_pEditPayeeListAction = new QAction("Payee List...", this);

    m_pMenuBar = new QMenuBar(this);

    // Top Level Menus
    m_pFileMenu = new QMenu("&File", m_pMenuBar);
    m_pEditMenu = new QMenu("&Edit", m_pMenuBar);
    m_pViewMenu = new QMenu("&View", m_pMenuBar);
    m_pMenuBar->addAction(m_pFileMenu->menuAction());
    m_pMenuBar->addAction(m_pEditMenu->menuAction());
    m_pMenuBar->addAction(m_pViewMenu->menuAction());

    // File Menu
    m_pFileMenu->addAction(m_pImportCsvAction);
    m_pFileMenu->addAction(m_pImportQifAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pExitAction);

    // Edit Menu
    m_pEditMenu->addAction(m_pEditAccountListAction);
    m_pEditMenu->addAction(m_pEditPayeeListAction);

    // View Menu
    m_pViewMenu->addAction(m_pAccountsDockWidget->toggleViewAction());

    setMenuBar(m_pMenuBar);

    QObject::connect(m_pImportCsvAction, &QAction::triggered, this, &CMainWindow::onImportCSV);
    QObject::connect(m_pImportQifAction, &QAction::triggered, this, &CMainWindow::onImportQIF);
    QObject::connect(m_pExitAction, &QAction::triggered, this, &CMainWindow::onExit);
    QObject::connect(m_pEditAccountListAction, &QAction::triggered, this, &CMainWindow::onEditAccountList);
    QObject::connect(m_pEditPayeeListAction, &QAction::triggered, this, &CMainWindow::onEditPayeeList);
}

void Queso::CMainWindow::initStatusBar()
{
    m_pStatusBar = new QStatusBar(this);
    setStatusBar(m_pStatusBar);
}

void Queso::CMainWindow::initAccountList()
{
    m_pAccountsDockWidget = new QDockWidget("Accounts", this);
    m_pAccountsDockWidget->setObjectName("AccountsDockWidget");
    m_pAccountsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_pAccountsDockWidget);
    QTreeView* pTreeView = new QTreeView(m_pAccountsDockWidget);
    m_pAccountsDockWidget->setWidget(pTreeView);

    pTreeView->setItemDelegate(new CAccountListDelegate(pTreeView));
    pTreeView->setModel(new CAccountListModel(false, pTreeView));
    pTreeView->setHeaderHidden(true);
    pTreeView->setRootIsDecorated(false);
    pTreeView->setItemsExpandable(false); // stop the user from expanding/collapsing the tree nodes
    pTreeView->setAlternatingRowColors(true);
    pTreeView->setIndentation(0);
    pTreeView->resizeColumnToContents(0);
    pTreeView->resizeColumnToContents(1);
    pTreeView->expandAll();
    QObject::connect(pTreeView, &QTreeView::activated, this, &CMainWindow::onAccountActivated);
}

void Queso::CMainWindow::onAccountActivated(const QModelIndex& index)
{
    const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());
    int iAccountId = pModel->accountId(index);
    if (iAccountId == 0)
        return;

    CAccount oAccount;
    if (!oAccount.dbRead(iAccountId))
        return;

    CAccount::Type eAccountType = oAccount.accountType();
    QString oName = oAccount.name();
    int iFinancialInstitutionId = oAccount.financialInstitutionId();

    CFinancialInstitution oFI;
    oFI.dbRead(iFinancialInstitutionId);
    QString oInstitution = oFI.name();

    qDebug() << iAccountId << oName << oInstitution;
    CMainWidget* pWidget = nullptr;
    switch (eAccountType)
    {
    case CAccount::Type::Banking:
        pWidget = makeWidget(MainWidget::Type::Banking);
        static_cast<CBankingWidget*>(pWidget)->setAccount(iAccountId);
        break;
    case CAccount::Type::Investment:
        pWidget = makeWidget(MainWidget::Type::Investment);
        break;
    case CAccount::Type::Retirement:
        pWidget = makeWidget(MainWidget::Type::Retirement);
        break;
    case CAccount::Type::Asset:
        pWidget = makeWidget(MainWidget::Type::Asset);
        break;
    case CAccount::Type::Liability:
        pWidget = makeWidget(MainWidget::Type::Liability);
        break;
    default:
        break;
    }

    if (pWidget)
    {
        m_pStackedWidget->addWidget(pWidget);
        m_pStackedWidget->setCurrentWidget(pWidget);
    }
}

Queso::CMainWidget* Queso::CMainWindow::makeWidget(const MainWidget::Type eWidgetType) const
{
    static std::map<MainWidget::Type, CMainWidget*> soMap;

    CMainWidget* pWidget = nullptr;
    auto iter = soMap.find(eWidgetType);
    if (iter != soMap.end())
        pWidget = iter->second;
    else
    {
        switch (eWidgetType)
        {
        case MainWidget::Type::FrontPage:
            pWidget = new CFrontPageWidget(m_pStackedWidget);
            break;
        case MainWidget::Type::Banking:
            pWidget = new CBankingWidget(m_pStackedWidget);
            break;
        case MainWidget::Type::Investment:
            break;
        case MainWidget::Type::Retirement:
            break;
        case MainWidget::Type::Asset:
            break;
        case MainWidget::Type::Liability:
            break;
        default:
            break;
        }

        if (pWidget)
            soMap[eWidgetType] = pWidget;
    }

    return pWidget;
}

void Queso::CMainWindow::closeEvent(QCloseEvent* pEvent)
{
    saveStateSettings();
    QMainWindow::closeEvent(pEvent);
}

void Queso::CMainWindow::saveStateSettings()
{
    CSettings oSettings;
    oSettings.setMainWindowGeometry(saveGeometry());
    oSettings.setMainWindowState(saveState());
}

void Queso::CMainWindow::restoreStateSettings()
{
    CSettings oSettings;
    restoreGeometry(oSettings.mainWindowGeometry());
    restoreState(oSettings.mainWindowState());
}

void Queso::CMainWindow::onImportCSV()
{
    qDebug() << "onImportCSV";

    CSettings oSettings;
    QString oFileName = QFileDialog::getOpenFileName(this, "Import CSV File", oSettings.importCSVPath(), "CSV Files (*.csv)");
    if (!oFileName.isNull())
    {
        oSettings.setImportCSVPath(QFileInfo(oFileName).path());
        CImportCSV oImporter;
        oImporter.import(oFileName);
    }
}

void Queso::CMainWindow::onImportQIF()
{
    qDebug() << "onImportQIF";

    CSettings oSettings;
    QString oFileName = QFileDialog::getOpenFileName(this, "Import QIF File", oSettings.importQIFPath(), "QIF Files (*.qif)");
    if (!oFileName.isNull())
    {
        oSettings.setImportQIFPath(QFileInfo(oFileName).path());
        CImportQIF oImporter;
        if (oImporter.import(oFileName))
        {
            qDebug() << "Successfully read QIF file";
            qDebug() << "Bank Transactions:" << oImporter.bankTransactions().size();
            qDebug() << "Cash Transactions:" << oImporter.cashTransactions().size();
            qDebug() << "Credit Card Transactions:" << oImporter.creditCardTransactions().size();
            qDebug() << "Investment Transactions:" << oImporter.investmentTransactions().size();
            qDebug() << "Other Asset Transactions:" << oImporter.otherAssetTransactions().size();
            qDebug() << "Other Liability Transactions:" << oImporter.otherLiabilityTransactions().size();
            qDebug() << "Accounts:" << oImporter.accounts().size();
            qDebug() << "Categories:" << oImporter.categories().size();
            qDebug() << "Classes:" << oImporter.classes().size();
            qDebug() << "Memorized:" << oImporter.memorized().size();
        }
    }
}

void Queso::CMainWindow::onExit()
{
    qDebug() << "onExit";
    close();
}

void Queso::CMainWindow::onEditAccountList()
{
    CAccountListEditorDlg dlg(this);
    dlg.exec();
}

void Queso::CMainWindow::onEditPayeeList()
{
    CPayeeListDlg dlg(this);
    dlg.exec();
}
