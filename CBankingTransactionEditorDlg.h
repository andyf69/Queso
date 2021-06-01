#pragma once
#include "ui_CBankingTransactionEditor.h"
#include <QtWidgets/QDialog>

namespace Queso
{
    class CBankingTransaction;

    class CBankingTransactionEditorDlg : public QDialog
    {
    public:
        explicit CBankingTransactionEditorDlg(QWidget* pParent, CBankingTransaction* pTrans);

    private:
        void onOk();
        void onCancel();

        Ui::CBankingTransactionEditor ui;
        CBankingTransaction* m_pBankingTransaction;
    };
}
