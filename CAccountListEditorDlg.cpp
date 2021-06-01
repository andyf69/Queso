#include "CAccountListEditorDlg.h"
#include "CAccountListDelegate.h"
#include "CAccountListModel.h"
#include <QtCore/QDebug>

Queso::CAccountListEditorDlg::CAccountListEditorDlg(QWidget* pParent)
    : QDialog(pParent)
{
    ui.setupUi(this);

    QObject::connect(ui.m_pAddButton, &QPushButton::clicked, this, &CAccountListEditorDlg::onAdd);
    QObject::connect(ui.m_pDeleteButton, &QPushButton::clicked, this, &CAccountListEditorDlg::onDelete);
    QObject::connect(ui.m_pCloseButton, &QPushButton::clicked, this, &CAccountListEditorDlg::onClose);

    initAccountList(ui.m_pAccountsTreeView);
}

void Queso::CAccountListEditorDlg::initAccountList(QTreeView* pTreeView)
{
    pTreeView->setItemDelegate(new CAccountListDelegate(pTreeView));
    pTreeView->setModel(new CAccountListModel(true, pTreeView));
    pTreeView->setHeaderHidden(true);
    pTreeView->setRootIsDecorated(false);
    pTreeView->setItemsExpandable(false); // stop the user from expanding/collapsing the tree nodes
    pTreeView->setAlternatingRowColors(true);
    pTreeView->setIndentation(0);
    pTreeView->resizeColumnToContents(0);
    pTreeView->resizeColumnToContents(1);
    pTreeView->expandAll();
    QObject::connect(pTreeView, &QTreeView::activated, this, &CAccountListEditorDlg::onAccountActivated);
}

void Queso::CAccountListEditorDlg::onAccountActivated(const QModelIndex& index)
{
    const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());
    int iAccountId = pModel->accountId(index);
    if (iAccountId == 0)
        return;

    // TODO: Edit the selected account
}

void Queso::CAccountListEditorDlg::onAdd()
{
    // TODO: Add a new account item
}

void Queso::CAccountListEditorDlg::onDelete()
{
    for (const auto index : ui.m_pAccountsTreeView->selectionModel()->selectedIndexes())
    {
        if (index.column() != 0)
            continue;

        const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());
        int iAccountId = pModel->accountId(index);
        if (iAccountId == 0)
            continue;

        // TODO: Delete is dangerous! Warning about transactions that will be deleted
        qDebug() << "onDelete account id:" << iAccountId;
    }
}

void Queso::CAccountListEditorDlg::onClose()
{
    close();
}
