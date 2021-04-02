#include "CMainWindow.h"
#include "CImportCSV.h"
#include "CImportQIF.h"
#include "CSettings.h"

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QtSql/QSqlRelationalTableModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTableView>


CMainWindow::CMainWindow()
    : QMainWindow()
{
    ui.setupUi(this);
    restoreStateSettings();
    initMenuActions();

    QSqlDatabase db = QSqlDatabase::database();

    QSqlRelationalTableModel* pModel = new QSqlRelationalTableModel(this, db);
    pModel->setTable("Account");
    pModel->setRelation(3, QSqlRelation("AccountType", "id", "Name"));
    pModel->setRelation(4, QSqlRelation("FinancialInstitution", "id", "Name"));
    bool ok = pModel->select();
    pModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
    pModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
    pModel->setHeaderData(2, Qt::Horizontal, tr("FI Acct Id"));
    pModel->setHeaderData(3, Qt::Horizontal, tr("Acct Type"));
    pModel->setHeaderData(4, Qt::Horizontal, tr("FI"));
    ui.m_pTableView->setModel(pModel);
    ui.m_pTableView->setItemDelegate(new QSqlRelationalDelegate(ui.m_pTableView));
    ui.m_pTableView->setAlternatingRowColors(true);

    initAccountList(ui.m_pAccountsListView);
}

void CMainWindow::initMenuActions()
{
    QObject::connect(ui.m_pActionImportCSV, &QAction::triggered, this, &CMainWindow::onImportCSV);
    QObject::connect(ui.m_pActionImportQIF, &QAction::triggered, this, &CMainWindow::onImportQIF);
    QObject::connect(ui.m_pActionExit, &QAction::triggered, this, &CMainWindow::onExit);
    QObject::connect(ui.m_pActionAccountList, &QAction::triggered, this, &CMainWindow::onAccountList);

    QObject::connect(ui.m_pCreatePBtn, &QPushButton::clicked, this, &CMainWindow::onCreate);
    QObject::connect(ui.m_pReadPBtn, &QPushButton::clicked, this, &CMainWindow::onRead);
    QObject::connect(ui.m_pUpdatePBtn, &QPushButton::clicked, this, &CMainWindow::onUpdate);
    QObject::connect(ui.m_pDeletePBtn, &QPushButton::clicked, this, &CMainWindow::onDelete);
}

void CMainWindow::initAccountList(QListView* pListView)
{
    class CAccountListDelegate: public QStyledItemDelegate
    {
    public:
        explicit CAccountListDelegate(QObject* pParent = nullptr) : QStyledItemDelegate(pParent) {}

    protected:
        void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
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

            // fetch our data from the model
            const QAbstractItemModel* pModel = index.model();
            QString oName = pModel->data(pModel->index(index.row(), 1)).toString();
            QString oInstitution = pModel->data(pModel->index(index.row(), 2)).toString();

            // draw the text
            QRect r1(opt.rect.left(), opt.rect.top(), opt.rect.width(), opt.rect.height() / 2);
            QRect r2(opt.rect.left(), opt.rect.top() + opt.rect.height() / 2, opt.rect.width(), opt.rect.height() / 2);

            pPainter->drawText(r1, opt.displayAlignment, oName);
            pPainter->drawText(r2, opt.displayAlignment, oInstitution);
        }

        QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
        {
            QSize result = QStyledItemDelegate::sizeHint(option, index);
            result.setHeight(result.height() * 2);
            return result;
        }
    };

    QSqlQueryModel* pModel = new QSqlQueryModel(pListView);
    pModel->setQuery("SELECT [Account].[Id]"
                     ", [Account].[Name]"
                     ", [FinancialInstitution].[Name] AS 'Institution'"
                     ", [Account].[AccountTypeId]"
                     "FROM [Account]"
                     "LEFT JOIN [FinancialInstitution] ON [Account].[FinancialInstitutionId] = [FinancialInstitution].[Id]"
                     "WHERE [Hidden] = 0"
                     "ORDER BY [Account].[AccountTypeId], [FinancialInstitution].[Name], [Account].[Name]");

    pListView->setItemDelegate(new CAccountListDelegate(pListView));
    pListView->setModel(pModel);
    pListView->setModelColumn(0);
    pListView->setAlternatingRowColors(true);

    QObject::connect(pListView, &QListView::activated, this, &CMainWindow::onAccountActivated);
}

void CMainWindow::onAccountActivated(const QModelIndex& index)
{
    const QAbstractItemModel* pModel = index.model();
    int iAccountId = pModel->data(pModel->index(index.row(), 0)).toInt();
    QString oName = pModel->data(pModel->index(index.row(), 1)).toString();
    QString oInstitution = pModel->data(pModel->index(index.row(), 2)).toString();

    qDebug() << iAccountId << oName << oInstitution;
}

void CMainWindow::closeEvent(QCloseEvent* pEvent)
{
    saveStateSettings();
    QMainWindow::closeEvent(pEvent);
}

void CMainWindow::saveStateSettings()
{
    CSettings oSettings;
    oSettings.setMainWindowGeometry(saveGeometry());
    oSettings.setMainWindowState(saveState());
    oSettings.setMainWindowVerticalSplitterState(ui.m_pLeftSplitter->saveState());
}

void CMainWindow::restoreStateSettings()
{
    CSettings oSettings;
    restoreGeometry(oSettings.mainWindowGeometry());
    restoreState(oSettings.mainWindowState());
    ui.m_pLeftSplitter->restoreState(oSettings.mainWindowVerticalSplitterState());
}

void CMainWindow::onCreate()
{
    qDebug() << "onCreate";
    QSqlQuery q;
    q.prepare("INSERT INTO Account (Name, FIAccountId, AccountTypeId, FinancialInstitutionId) "
              "VALUES(:name, :fiaid, :atid, :fiid)");
    q.bindValue(":name", "Test Account");
    q.bindValue(":fiaid", 1234);
    q.bindValue(":atid", 1);
    q.bindValue(":fiid", 1);
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onRead()
{
    qDebug() << "onRead";
    QSqlQuery q;
    q.prepare("SELECT Id, Name, FIAccountId, AccountTypeId, FinancialInstitutionId "
              "FROM Account "
              "WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";

    if (q.first())
    {
        qDebug() << "Id: " << q.value("Id").toInt();
        qDebug() << "Name: " << q.value("Name").toString();
        qDebug() << "FIAccountId: " << q.value("FIAccountId").toInt();
        qDebug() << "AccountTypeId: " << q.value("AccountTypeId").toInt();
        qDebug() << "FinancialInstitutionId: " << q.value("FinancialInstitutionId").toInt();
    }
}

void CMainWindow::onUpdate()
{
    qDebug() << "onUpdate";
    QSqlQuery q;
    q.prepare("UPDATE Account "
              "SET FIAccountId=:fiaid, AccountTypeId=:atid "
              "WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    q.bindValue(":fiaid", 4321);
    q.bindValue(":atid", 2);
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onDelete()
{
    qDebug() << "onDelete";
    QSqlQuery q;
    q.prepare("DELETE FROM Account WHERE Name=:name");
    q.bindValue(":name", "Test Account");
    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";
}

void CMainWindow::onImportCSV()
{
    qDebug() << "onImportCSV";

    CSettings oSettings;
    QString oFileName = QFileDialog::getOpenFileName(this, "Import CSV File", oSettings.importCSVPath(), "CSV Files (*.csv)");
    if (!oFileName.isNull())
    {
        oSettings.setImportCSVPath(QFileInfo(oFileName).path());
        CImportCSV oImporter;
        oImporter.import(oFileName);
    }
}

void CMainWindow::onImportQIF()
{
    qDebug() << "onImportQIF";

    CSettings oSettings;
    QString oFileName = QFileDialog::getOpenFileName(this, "Import QIF File", oSettings.importQIFPath(), "QIF Files (*.qif)");
    if (!oFileName.isNull())
    {
        oSettings.setImportQIFPath(QFileInfo(oFileName).path());
        CImportQIF oImporter;
        if (oImporter.import(oFileName))
        {
            qDebug() << "Successfully read QIF file";
            qDebug() << "Bank Transactions:" << oImporter.bankTransactions().size();
            qDebug() << "Cash Transactions:" << oImporter.cashTransactions().size();
            qDebug() << "Credit Card Transactions:" << oImporter.creditCardTransactions().size();
            qDebug() << "Investment Transactions:" << oImporter.investmentTransactions().size();
            qDebug() << "Other Asset Transactions:" << oImporter.otherAssetTransactions().size();
            qDebug() << "Other Liability Transactions:" << oImporter.otherLiabilityTransactions().size();
            qDebug() << "Accounts:" << oImporter.accounts().size();
            qDebug() << "Categories:" << oImporter.categories().size();
            qDebug() << "Classes:" << oImporter.classes().size();
            qDebug() << "Memorized:" << oImporter.memorized().size();
        }
    }
}

void CMainWindow::onExit()
{
    qDebug() << "onExit";
    close();
}

void CMainWindow::onAccountList()
{
    qDebug() << "onAccountList: TODO";
}
