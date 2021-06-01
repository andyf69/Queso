#pragma once
#include <QtWidgets/QWidget>

namespace Queso
{
    namespace MainWidget
    {
        enum class Type
        {
            FrontPage,
            Banking,
            Investment,
            Retirement,
            Asset,
            Liability
        };
    }

    class CMainWidget : public QWidget
    {
    public:
        explicit CMainWidget(QWidget* pParent) : QWidget(pParent) {}

        virtual MainWidget::Type witdgetType() const = 0;
    };
}
