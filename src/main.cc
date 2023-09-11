#include <QApplication>

#include "controller/controller.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  s21::Controller::GetInstance().ShowWindow();
  return QApplication::exec();
}
