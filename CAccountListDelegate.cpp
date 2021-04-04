#include "CAccountListDelegate.h"
#include "CAccountListModel.h"
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

void CAccountListDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(pPainter, option, index);

    const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());

    if (pModel->accountId(index) == 0)
    {
        QStyleOptionViewItem opt(option);
        QStyledItemDelegate::initStyleOption(&opt, index);
        QPalette::ColorGroup cg = QPalette::Normal;

        pPainter->setPen(opt.palette.color(cg, QPalette::Text));
        pPainter->drawLine(opt.rect.topLeft(), opt.rect.topRight());
        pPainter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());
    }
}
