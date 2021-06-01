#pragma once
#include "CMainWidget.h"
#include "ui_CBankingWidget.h"

namespace Queso
{
    class CBankingWidget : public CMainWidget
    {
    public:
        explicit CBankingWidget(QWidget* pParent);

        virtual MainWidget::Type witdgetType() const { return MainWidget::Type::Banking; }

        void setAccount(const int iAccountId);

    private:
        Ui::CBankingWidget ui;

        void onEdit(const QModelIndex& oIndex);
    };
}
