#include "CMainWindow.h"
#include "CAccount.h"
#include "CAccountListDelegate.h"
#include "CAccountListEditorDlg.h"
#include "CAccountListModel.h"
#include "CFinancialInstitution.h"
#include "CImportCSV.h"
#include "CImportQIF.h"
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
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>


CMainWindow::CMainWindow()
    : QMainWindow()
{
    ui.setupUi(this);

    initMenus();
    initStatusBar();
    initAccountList();

    QObject::connect(ui.m_pCreatePBtn, &QPushButton::clicked, this, &CMainWindow::onCreate);
    QObject::connect(ui.m_pReadPBtn, &QPushButton::clicked, this, &CMainWindow::onRead);
    QObject::connect(ui.m_pUpdatePBtn, &QPushButton::clicked, this, &CMainWindow::onUpdate);
    QObject::connect(ui.m_pDeletePBtn, &QPushButton::clicked, this, &CMainWindow::onDelete);

    restoreStateSettings();

    QSqlDatabase db = QSqlDatabase::database();

    QSqlRelationalTableModel* pModel = new QSqlRelationalTableModel(this, db);
    pModel->setTable("Account");
    pModel->setRelation(4, QSqlRelation("FinancialInstitution", "id", "Name"));
    bool ok = pModel->select();
    pModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    pModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    pModel->setHeaderData(2, Qt::Horizontal, tr("FI Acct Id"));
    pModel->setHeaderData(3, Qt::Horizontal, tr("Acct Type"));
    pModel->setHeaderData(4, Qt::Horizontal, tr("FI"));
    ui.m_pTableView->setModel(pModel);
    ui.m_pTableView->setItemDelegate(new QSqlRelationalDelegate(ui.m_pTableView));
    ui.m_pTableView->setAlternatingRowColors(true);

}

void CMainWindow::initMenus()
{
    m_pImportCsvAction = new QAction("Import CSV...", this);
    m_pImportQifAction = new QAction("Import QIF...", this);
    m_pExitAction = new QAction("Exit", this);
    m_pAccountListAction = new QAction("Account List...", this);

    m_pMenuBar = new QMenuBar(this);

    // Top Level Menus
    m_pFileMenu = new QMenu("&File", m_pMenuBar);
    m_pEditMenu = new QMenu("&Edit", m_pMenuBar);
    m_pMenuBar->addAction(m_pFileMenu->menuAction());
    m_pMenuBar->addAction(m_pEditMenu->menuAction());

    // File Menu
    m_pFileMenu->addAction(m_pImportCsvAction);
    m_pFileMenu->addAction(m_pImportQifAction);
    m_pFileMenu->addSeparator();
    m_pFileMenu->addAction(m_pExitAction);

    // Edit Menu
    m_pEditMenu->addAction(m_pAccountListAction);

    setMenuBar(m_pMenuBar);

    QObject::connect(m_pImportCsvAction, &QAction::triggered, this, &CMainWindow::onImportCSV);
    QObject::connect(m_pImportQifAction, &QAction::triggered, this, &CMainWindow::onImportQIF);
    QObject::connect(m_pExitAction, &QAction::triggered, this, &CMainWindow::onExit);
    QObject::connect(m_pAccountListAction, &QAction::triggered, this, &CMainWindow::onAccountList);
}

void CMainWindow::initStatusBar()
{
    m_pStatusBar = new QStatusBar(this);
    setStatusBar(m_pStatusBar);
}

void CMainWindow::initAccountList()
{
    m_pAccountsDockWidget = new QDockWidget("Accounts", this);
    m_pAccountsDockWidget->setObjectName("AccountsDockWidget");
    m_pAccountsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_pAccountsDockWidget);
    QTreeView* pTreeView = new QTreeView(m_pAccountsDockWidget);
    m_pAccountsDockWidget->setWidget(pTreeView);
    //viewMenu->addAction(dock->toggleViewAction());

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

void CMainWindow::onAccountActivated(const QModelIndex& index)
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
    switch (eAccountType)
    {
    case CAccount::Type::Banking:
        break;
    case CAccount::Type::Investment:
        break;
    case CAccount::Type::Retirement:
        break;
    case CAccount::Type::Asset:
        break;
    case CAccount::Type::Liability:
        break;
    default:
        break;
    }
}

void CMainWindow::closeEvent(QCloseEvent* pEvent)
{
    saveStateSettings();
    QMainWindow::closeEvent(pEvent);
}

void CMainWindow::saveStateSettings()
{
    CSettings oSettings;
    oSettings.setMainWindowGeometry(saveGeometry());
    oSettings.setMainWindowState(saveState());
}

void CMainWindow::restoreStateSettings()
{
    CSettings oSettings;
    restoreGeometry(oSettings.mainWindowGeometry());
    restoreState(oSettings.mainWindowState());
}

void CMainWindow::onCreate()
{
    qDebug() << "onCreate";
    QSqlQuery q;
    q.prepare("INSERT INTO Account (Name, FIAccountId, AccountTypeId, FinancialInstitutionId) "
              "VALUES(:name, :fiaid, :atid, :fiid)");
    q.bindValue(":name", "Test Account");
    q.bindValue(":fiaid", 1234);
    q.bindValue(":atid", 1);
    q.bindValue(":fiid", 1);
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onRead()
{
    qDebug() << "onRead";
    QSqlQuery q;
    q.prepare("SELECT Id, Name, FIAccountId, AccountTypeId, FinancialInstitutionId "
              "FROM Account "
              "WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";

    if (q.first())
    {
        qDebug() << "Id: " << q.value("Id").toInt();
        qDebug() << "Name: " << q.value("Name").toString();
        qDebug() << "FIAccountId: " << q.value("FIAccountId").toInt();
        qDebug() << "AccountTypeId: " << q.value("AccountTypeId").toInt();
        qDebug() << "FinancialInstitutionId: " << q.value("FinancialInstitutionId").toInt();
    }
}

void CMainWindow::onUpdate()
{
    qDebug() << "onUpdate";
    QSqlQuery q;
    q.prepare("UPDATE Account "
              "SET FIAccountId=:fiaid, AccountTypeId=:atid "
              "WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    q.bindValue(":fiaid", 4321);
    q.bindValue(":atid", 2);
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onDelete()
{
    qDebug() << "onDelete";
    QSqlQuery q;
    q.prepare("DELETE FROM Account WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onImportCSV()
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

void CMainWindow::onImportQIF()
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

void CMainWindow::onExit()
{
    qDebug() << "onExit";
    close();
}

void CMainWindow::onAccountList()
{
    qDebug() << "onAccountList: TODO";
    CAccountListEditorDlg dlg(this);
    dlg.exec();
}
