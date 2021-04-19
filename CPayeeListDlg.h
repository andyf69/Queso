#pragma once
#include "ui_CPayeeListDlg.h"
#include <QtWidgets/QDialog>

class CPayeeListDlg : public QDialog
{
public:
	explicit CPayeeListDlg(QWidget* pParent = nullptr);

private:
    void initPayeeList(QListView* pListView);

    void onPayeeActivated(const QModelIndex& index);

    void onAdd();
    void onClose();
    
    Ui::CPayeeListDlg ui;
};

