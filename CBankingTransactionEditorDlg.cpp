#include "CBankingTransactionEditorDlg.h"
#include "CBankingTransaction.h"
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

namespace
{
    static int lookup_model_index(QSqlTableModel* pModel, int fieldIndex, int value)
    {
        int rc = pModel->rowCount();
        for (int i = 0; i < rc; ++i)
        {
            if (pModel->record(i).value(fieldIndex).toInt() == value)
                return i;
        }

        return -1;
    }
}

Queso::CBankingTransactionEditorDlg::CBankingTransactionEditorDlg(QWidget* pParent, CBankingTransaction* pTrans)
    : QDialog(pParent)
{
    m_pBankingTransaction = pTrans;

    ui.setupUi(this);

    QSqlTableModel* pPayeeModel = new QSqlTableModel();
    pPayeeModel->setTable("Payee");
    pPayeeModel->setSort(1, Qt::AscendingOrder);
    pPayeeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    pPayeeModel->select();

    ui.m_pPayeeCombo->setEditable(true);
    ui.m_pPayeeCombo->setModel(pPayeeModel);
    ui.m_pPayeeCombo->setModelColumn(pPayeeModel->fieldIndex("Name"));
    ui.m_pPayeeCombo->setCurrentIndex(lookup_model_index(pPayeeModel, pPayeeModel->fieldIndex("Id"), pTrans->payeeId()));

    ui.m_pDateEdit->setDate(pTrans->date());
    ui.m_pAmountLineEdit->setText(QString::fromStdString(pTrans->amount().asString()));
    ui.m_pMemoLineEdit->setText(pTrans->memo());
    ui.m_pCheckNumLineEdit->setText(pTrans->checkNumber() > 0 ? QString::number(pTrans->checkNumber()) : QString());
    ui.m_pReconciledCombo->setCurrentText(QString(pTrans->reconciled()));

    QObject::connect(ui.m_pOkBtn, &QPushButton::clicked, this, &CBankingTransactionEditorDlg::onOk);
    QObject::connect(ui.m_pCancelBtn, &QPushButton::clicked, this, &CBankingTransactionEditorDlg::onCancel);
}

void Queso::CBankingTransactionEditorDlg::onOk()
{
    QDialog::accept();
}

void Queso::CBankingTransactionEditorDlg::onCancel()
{
    QDialog::reject();
}

