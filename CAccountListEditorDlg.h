#pragma once
#include "ui_CAccountListEditor.h"
#include <QtWidgets/QDialog>

namespace Queso
{
    class CAccountListEditorDlg : public QDialog
    {
    public:
        explicit CAccountListEditorDlg(QWidget* pParent);

    private:
        void initAccountList(QTreeView* pTreeView);

        void onAccountActivated(const QModelIndex& index);

        void onAdd();
        void onDelete();
        void onClose();

        Ui::AccountListEditor ui;
    };
}
