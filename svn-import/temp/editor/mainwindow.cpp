#include "mainwindow.h"
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QTabWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QDebug>

#include "filedock.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    FileDock *filedock = new FileDock("Files", this);
    filedock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QTreeWidget *tree = new QTreeWidget(this);

    filedock->setWidget(tree);

    addDockWidget(Qt::LeftDockWidgetArea, filedock);

    QTabWidget *tab = new QTabWidget(this);


    this->setCentralWidget(tab);

    editor = new TextEdit(this);
    tab->addTab(editor,"Hello");

    QToolBar *toolbar = new QToolBar("mainbar", this);
    this->addToolBar(toolbar);

    QMenuBar *menubar = new QMenuBar(this);
    this->setMenuBar(menubar);

    QMenu *projectMenu = new QMenu(menubar);
    projectMenu->setTitle("Project");

    menubar->addMenu(projectMenu);

    QMenu *buildMenu = new QMenu(menubar);
    buildMenu->setTitle("Build");
    menubar->addMenu(buildMenu);

    QMenu *deployMenu = new QMenu(menubar);
    deployMenu->setTitle("Deploy");
    menubar->addMenu(deployMenu);

    open = new QAction(QIcon(":/images/open.png"), "Open",this);
    open->setStatusTip("Open project");
    open->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    save = new QAction(QIcon(":/images/save.png"), "Save", this);
    save->setStatusTip("Save project");
    save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    create = new QAction(QIcon(":/images/new.png"), "Create", this);
    create->setStatusTip("Create project");
    create->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    toolbar->addAction(create);
    toolbar->addAction(open);
    toolbar->addAction(save);

    projectMenu->addAction(create);
    projectMenu->addAction(open);
    projectMenu->addAction(save);

    connect(open, SIGNAL(triggered()), this, SLOT(slotOpen()));
    connect(save, SIGNAL(triggered()), this, SLOT(slotSave()));

}


void MainWindow::slotSave()
{
    this->editor->save();
}

void MainWindow::slotOpen()
{
    qDebug()<<"Open";
}
