#include "CBankingWidget.h"
#include "CAccount.h"
#include "CBankingRegisterDelegate.h"
#include "CBankingRegisterModel.h"
#include <QtCore/QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlRelationalTableModel>

CBankingWidget::CBankingWidget(QWidget* pParent)
    :QWidget(pParent)
{
    ui.setupUi(this);
    ui.m_pRegisterTableView->setAlternatingRowColors(true);
    ui.m_pRegisterTableView->verticalHeader()->setVisible(false);
    ui.m_pRegisterTableView->setItemDelegate(new CBankingRegisterDelegate(ui.m_pRegisterTableView));
}

void CBankingWidget::setAccount(const int iAccountId)
{
    CAccount oAccount;
    oAccount.dbRead(iAccountId);
    ui.m_pAccountName->setText(oAccount.name());

    CBankingRegisterModel* pModel = new CBankingRegisterModel(this);
    pModel->setAccount(iAccountId);

    ui.m_pRegisterTableView->setModel(pModel);
    ui.m_pRegisterTableView->hideColumn(static_cast<int>(CBankingRegisterModel::Column::Id));
    ui.m_pRegisterTableView->hideColumn(static_cast<int>(CBankingRegisterModel::Column::SequenceNum));
    ui.m_pRegisterTableView->scrollToBottom();
}
