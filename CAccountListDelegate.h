#pragma once
#include <QtWidgets/QStyledItemDelegate>

namespace Queso
{
    class CAccountListDelegate : public QStyledItemDelegate
    {
    public:
        explicit CAccountListDelegate(QObject* pParent = nullptr) : QStyledItemDelegate(pParent) {}
        ~CAccountListDelegate() = default;

    protected:
        virtual void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    };
}
