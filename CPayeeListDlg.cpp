#include "CPayeeListDlg.h"
#include <QtSql/QSqlTableModel>

CPayeeListDlg::CPayeeListDlg(QWidget* pParent)
    : QDialog(pParent)
{
    ui.setupUi(this);

    QObject::connect(ui.m_pAddButton, &QPushButton::clicked, this, &CPayeeListDlg::onAdd);
    QObject::connect(ui.m_pCloseButton, &QPushButton::clicked, this, &CPayeeListDlg::onClose);

    initPayeeList(ui.m_pListView);
}

void CPayeeListDlg::initPayeeList(QListView* pListView)
{
    QSqlTableModel* pModel = new QSqlTableModel(this);
    pModel->setTable("Payee");
    pModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    pModel->setSort(1, Qt::AscendingOrder);
    pModel->select();

    pListView->setModel(pModel);
    pListView->setModelColumn(1);

    //pTreeView->setItemDelegate(new CAccountListDelegate(pTreeView));
    //pTreeView->setModel(new CAccountListModel(true, pTreeView));
    //pTreeView->setHeaderHidden(true);
    //pTreeView->setRootIsDecorated(false);
    //pTreeView->setItemsExpandable(false); // stop the user from expanding/collapsing the tree nodes
    //pTreeView->setAlternatingRowColors(true);
    //pTreeView->setIndentation(0);
    //pTreeView->resizeColumnToContents(0);
    //pTreeView->resizeColumnToContents(1);
    //pTreeView->expandAll();
    QObject::connect(pListView, &QListView::activated, this, &CPayeeListDlg::onPayeeActivated);
}

void CPayeeListDlg::onPayeeActivated(const QModelIndex& index)
{
    //const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());
    //int iAccountId = pModel->accountId(index);
    //if (iAccountId == 0)
    //    return;

    // TODO: Edit the selected payee
}

void CPayeeListDlg::onAdd()
{
    // TODO: Add a new payee item
}

void CPayeeListDlg::onClose()
{
    close();
}
