#include "CMainWindow.h"
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Feeping Widgets");
    a.setApplicationName("Queso");

    const QString oConnectionString("DRIVER={SQL SERVER};SERVER=.\\SQLExpress;Integrated Security=true;DATABASE=Queso;Trusted_Connection=true;");
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(oConnectionString);
    if (!db.open())
    {
        qDebug() << "Failed to open database connection";
        qDebug() << db.lastError().text();
        return -1;
    }

    CMainWindow w;
    w.show();
    return a.exec();
}
