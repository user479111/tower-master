#include <QApplication>
#include <QSharedPointer>

#include "TowerMaster.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // hide oroginal cursor
    a.setOverrideCursor(QCursor(Qt::BlankCursor));

    QSharedPointer<TowerMaster> towerMaster = QSharedPointer<TowerMaster>(new TowerMaster());
    towerMaster->show();

    return a.exec();
}
