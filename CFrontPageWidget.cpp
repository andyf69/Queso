#include "CFrontPageWidget.h"
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlRelationalTableModel>

Queso::CFrontPageWidget::CFrontPageWidget(QWidget* pParent)
    :CMainWidget(pParent)
{
    ui.setupUi(this);

    QObject::connect(ui.m_pCreatePBtn, &QPushButton::clicked, this, &CFrontPageWidget::onCreate);
    QObject::connect(ui.m_pReadPBtn, &QPushButton::clicked, this, &CFrontPageWidget::onRead);
    QObject::connect(ui.m_pUpdatePBtn, &QPushButton::clicked, this, &CFrontPageWidget::onUpdate);
    QObject::connect(ui.m_pDeletePBtn, &QPushButton::clicked, this, &CFrontPageWidget::onDelete);

    QSqlDatabase db = QSqlDatabase::database();

    QSqlRelationalTableModel* pModel = new QSqlRelationalTableModel(this, db);
    pModel->setTable("Account");
    pModel->setRelation(4, QSqlRelation("FinancialInstitution", "id", "Name"));
    bool ok = pModel->select();
    pModel->setHeaderData(0, Qt::Horizontal, "Id");
    pModel->setHeaderData(1, Qt::Horizontal, "Name");
    pModel->setHeaderData(2, Qt::Horizontal, "FI Acct Id");
    pModel->setHeaderData(3, Qt::Horizontal, "Acct Type");
    pModel->setHeaderData(4, Qt::Horizontal, "FI");
    ui.m_pTableView->setModel(pModel);
    ui.m_pTableView->setItemDelegate(new QSqlRelationalDelegate(ui.m_pTableView));
    ui.m_pTableView->setAlternatingRowColors(true);
}

void Queso::CFrontPageWidget::onCreate()
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

void Queso::CFrontPageWidget::onRead()
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

void Queso::CFrontPageWidget::onUpdate()
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

void Queso::CFrontPageWidget::onDelete()
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

