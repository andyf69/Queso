#pragma once
#include "ui_CPayeeListDlg.h"
#include <QtWidgets/QDialog>

class QSqlTableModel;

class CPayeeListDlg : public QDialog
{
public:
	explicit CPayeeListDlg(QWidget* pParent = nullptr);

private:
    void initPayeeList(QListView* pListView);

    void onPayeeActivated(const QModelIndex& oIndex);

    void onAdd();
    void onEdit();
    void onClose();
    
    Ui::CPayeeListDlg ui;
    QSqlTableModel* m_pModel;
};

