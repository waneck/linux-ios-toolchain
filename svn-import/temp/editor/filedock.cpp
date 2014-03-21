#include "filedock.h"
#include <QStyleOptionDockWidget>

FileDock::FileDock(QString title, QWidget *parent) :
    QDockWidget(title,parent)
{
    QStyleOptionDockWidget *dockstyle = new QStyleOptionDockWidget();
    dockstyle->closable=false;
    dockstyle->floatable=false;
    dockstyle->movable=false;
    this->initStyleOption(dockstyle);

}
