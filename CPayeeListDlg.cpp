#include "CPayeeListDlg.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QInputDialog>

CPayeeListDlg::CPayeeListDlg(QWidget* pParent)
    : QDialog(pParent)
{
    ui.setupUi(this);

    QObject::connect(ui.m_pAddButton, &QPushButton::clicked, this, &CPayeeListDlg::onAdd);
    QObject::connect(ui.m_pEditButton, &QPushButton::clicked, this, &CPayeeListDlg::onEdit);
    QObject::connect(ui.m_pCloseButton, &QPushButton::clicked, this, &CPayeeListDlg::onClose);

    initPayeeList(ui.m_pListView);
}

void CPayeeListDlg::initPayeeList(QListView* pListView)
{
    m_pModel = new QSqlTableModel(this);
    m_pModel->setTable("Payee");
    m_pModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_pModel->setSort(1, Qt::AscendingOrder);
    m_pModel->select();

    pListView->setModel(m_pModel);
    pListView->setModelColumn(1);
    pListView->setAlternatingRowColors(true);
    pListView->setEditTriggers(QListView::NoEditTriggers);
    QObject::connect(pListView, &QListView::activated, this, &CPayeeListDlg::onPayeeActivated);
}

void CPayeeListDlg::onPayeeActivated(const QModelIndex& oIndex)
{
    QString oPayeeName = m_pModel->data(oIndex).toString();
    int iPayeeId = m_pModel->data(oIndex.siblingAtColumn(0)).toInt();
    QString oNewPayee = QInputDialog::getText(this, "Edit Payee", "Payee Name", QLineEdit::Normal, oPayeeName).trimmed();
    if (!oNewPayee.isEmpty() && oNewPayee != oPayeeName)
    {
        m_pModel->setData(oIndex, oNewPayee, Qt::EditRole);
        m_pModel->submitAll();
    }
}

void CPayeeListDlg::onEdit()
{
    QItemSelectionModel* pSelectionModel = ui.m_pListView->selectionModel();
    if (pSelectionModel->hasSelection())
    {
        QModelIndexList oIndexList = pSelectionModel->selectedIndexes();
        const QModelIndex& oIndex = oIndexList.front();
        QString oPayeeName = m_pModel->data(oIndex).toString();
        int iPayeeId = m_pModel->data(oIndex.siblingAtColumn(0)).toInt();
        QString oNewPayee = QInputDialog::getText(this, "Edit Payee", "Payee Name", QLineEdit::Normal, oPayeeName).trimmed();
        if (!oNewPayee.isEmpty() && oNewPayee != oPayeeName)
        {
            m_pModel->setData(oIndex, oNewPayee, Qt::EditRole);
            m_pModel->submitAll();
        }
    }
}

void CPayeeListDlg::onAdd()
{
    QString oNewPayee = QInputDialog::getText(this, "Add Payee", "Payee Name", QLineEdit::Normal).trimmed();
    if (!oNewPayee.isEmpty())
    {
        QSqlRecord oRecord = m_pModel->record();
        oRecord.remove(oRecord.indexOf("Id"));
        oRecord.setValue("Name", oNewPayee);
        m_pModel->insertRecord(-1, oRecord);
        m_pModel->submitAll();
    }
}

void CPayeeListDlg::onClose()
{
    close();
}
