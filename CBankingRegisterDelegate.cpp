#include "CBankingRegisterDelegate.h"
#include "CBankingRegisterModel.h"
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

void Queso::CBankingRegisterDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    QStyledItemDelegate::initStyleOption(&opt, index);

    switch (static_cast<CBankingRegisterModel::Column>(index.column()))
    {
    case CBankingRegisterModel::Column::Amount:
    case CBankingRegisterModel::Column::Balance:
        opt.displayAlignment = Qt::AlignVCenter | Qt::AlignRight;
        if (opt.text.startsWith('-'))
            opt.palette.setColor(QPalette::Text, Qt::red);
        else
            opt.palette.setColor(QPalette::Text, Qt::darkGreen);
        break;
    default:
        break;
    }

    QStyledItemDelegate::paint(pPainter, opt, index);

    //const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());

    //if (pModel->accountId(index) == 0)
    //{
    //    QStyleOptionViewItem opt(option);
    //    QStyledItemDelegate::initStyleOption(&opt, index);
    //    QPalette::ColorGroup cg = QPalette::Normal;

    //    pPainter->setPen(opt.palette.color(cg, QPalette::Text));
    //    pPainter->drawLine(opt.rect.topLeft(), opt.rect.topRight());
    //    pPainter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());
    //}
}
