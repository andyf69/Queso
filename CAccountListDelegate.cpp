#include "CAccountListDelegate.h"
#include "CAccountListModel.h"
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

void CAccountListDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    QStyledItemDelegate::initStyleOption(&opt, index);

    // draw correct background
    opt.text = "";
    QStyle* pStyle = opt.widget ? opt.widget->style() : QApplication::style();
    pStyle->drawPrimitive(QStyle::PE_PanelItemViewRow, &opt, pPainter, opt.widget);

    QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        cg = QPalette::Inactive;

    // set pen color
    if (opt.state & QStyle::State_Selected)
        pPainter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
    else
        pPainter->setPen(opt.palette.color(cg, QPalette::Text));

    // fetch our data model
    const CAccountListModel* pModel = static_cast<const CAccountListModel*>(index.model());

    // draw the text
    QRect r1(opt.rect.left(), opt.rect.top(), opt.rect.width(), opt.rect.height() / 2);
    QRect r2(opt.rect.left(), opt.rect.top() + opt.rect.height() / 2, opt.rect.width(), opt.rect.height() / 2);

    pPainter->drawText(r1, opt.displayAlignment, pModel->accountName(index.row()));
    pPainter->drawText(r2, opt.displayAlignment, pModel->InstitutionName(index.row()));
}

QSize CAccountListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(result.height() * 2);
    return result;
}
