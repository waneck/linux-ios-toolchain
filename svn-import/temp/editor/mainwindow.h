#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "textedit.h"
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    
public slots:
    void slotOpen();
    void slotSave();
private:
    TextEdit *editor;
    QAction *open;
    QAction *save;
    QAction *create;

    
};

#endif // MAINWINDOW_H
