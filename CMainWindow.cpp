#include "CMainWindow.h"
#include "CAccount.h"
#include "CAccountListDelegate.h"
#include "CAccountListModel.h"
#include "CImportCSV.h"
#include "CImportQIF.h"
#include "CSettings.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlRelationalTableModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>


CMainWindow::CMainWindow()
    : QMainWindow()
{
    ui.setupUi(this);
    restoreStateSettings();
    initMenuActions();

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

    initAccountList(ui.m_pAccountsTreeView);
}

void CMainWindow::initMenuActions()
{
    QObject::connect(ui.m_pActionImportCSV, &QAction::triggered, this, &CMainWindow::onImportCSV);
    QObject::connect(ui.m_pActionImportQIF, &QAction::triggered, this, &CMainWindow::onImportQIF);
    QObject::connect(ui.m_pActionExit, &QAction::triggered, this, &CMainWindow::onExit);
    QObject::connect(ui.m_pActionAccountList, &QAction::triggered, this, &CMainWindow::onAccountList);

    QObject::connect(ui.m_pCreatePBtn, &QPushButton::clicked, this, &CMainWindow::onCreate);
    QObject::connect(ui.m_pReadPBtn, &QPushButton::clicked, this, &CMainWindow::onRead);
    QObject::connect(ui.m_pUpdatePBtn, &QPushButton::clicked, this, &CMainWindow::onUpdate);
    QObject::connect(ui.m_pDeletePBtn, &QPushButton::clicked, this, &CMainWindow::onDelete);
}

void CMainWindow::initAccountList(QTreeView* pTreeView)
{
    pTreeView->setItemDelegate(new CAccountListDelegate(pTreeView));
    pTreeView->setModel(new CAccountListModel(pTreeView));
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

    int iAccountTypeId = pModel->accountTypeId(index);
    QString oName = pModel->accountName(index);
    QString oInstitution = pModel->InstitutionName(index);

    qDebug() << iAccountId << oName << oInstitution;
    switch (static_cast<CAccount::Type>(iAccountTypeId))
    {
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
    oSettings.setMainWindowVerticalSplitterState(ui.m_pLeftSplitter->saveState());
}

void CMainWindow::restoreStateSettings()
{
    CSettings oSettings;
    restoreGeometry(oSettings.mainWindowGeometry());
    restoreState(oSettings.mainWindowState());
    ui.m_pLeftSplitter->restoreState(oSettings.mainWindowVerticalSplitterState());
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
}
