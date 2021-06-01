#pragma once
#include "CMainWidget.h"
#include "ui_CFrontPageWidget.h"

namespace Queso
{
    class CFrontPageWidget : public CMainWidget
    {
    public:
        explicit CFrontPageWidget(QWidget* pParent);

        virtual MainWidget::Type witdgetType() const { return MainWidget::Type::FrontPage; }

    private:
        Ui::CFrontPageWidget ui;

        void onCreate();
        void onRead();
        void onUpdate();
        void onDelete();
    };
}
