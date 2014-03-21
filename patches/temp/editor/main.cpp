#include <QApplication>

#include "mainwindow.h"


int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  MainWindow *window = new MainWindow();

  window->setMinimumSize(800, 600);
  //editor->show();
  window->show();
  return app.exec();
}
