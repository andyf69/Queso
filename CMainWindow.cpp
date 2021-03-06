#include "CMainWindow.h"
#include "CImportCSV.h"
#include "CImportQIF.h"
#include "CSettings.h"

#include <QtCore/QDebug>
//#include <QtCore/QFile>
//#include <QtCore/QTextStream>
#include <QtSql/QSqlDatabase>
//#include <QtSql/QSqlError>
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
    initMenuActions();

    QSqlDatabase db = QSqlDatabase::database();

    QSqlRelationalTableModel* pModel = new QSqlRelationalTableModel(this, db);
    pModel->setTable("Account");
    pModel->setRelation(3, QSqlRelation("AccountType", "id", "Name"));
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

void CMainWindow::initMenuActions()
{
    QObject::connect(ui.m_pActionImportCSV, &QAction::triggered, this, &CMainWindow::onImportCSV);
    QObject::connect(ui.m_pActionImportQIF, &QAction::triggered, this, &CMainWindow::onImportQIF);
    QObject::connect(ui.m_pActionExit, &QAction::triggered, this, &CMainWindow::onExit);

    QObject::connect(ui.m_pCreatePBtn, &QPushButton::clicked, this, &CMainWindow::onCreate);
    QObject::connect(ui.m_pReadPBtn, &QPushButton::clicked, this, &CMainWindow::onRead);
    QObject::connect(ui.m_pUpdatePBtn, &QPushButton::clicked, this, &CMainWindow::onUpdate);
    QObject::connect(ui.m_pDeletePBtn, &QPushButton::clicked, this, &CMainWindow::onDelete);
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
        oImporter.import(oFileName);
    }
}

void CMainWindow::onExit()
{
    qDebug() << "onExit";
    close();
}
