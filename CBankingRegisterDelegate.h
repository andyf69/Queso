#pragma once
#include <QtWidgets/QStyledItemDelegate>

class CBankingRegisterDelegate : public QStyledItemDelegate
{
public:
    explicit CBankingRegisterDelegate(QObject* pParent = nullptr) : QStyledItemDelegate(pParent) {}
    ~CBankingRegisterDelegate() = default;

protected:
    virtual void paint(QPainter * pPainter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

