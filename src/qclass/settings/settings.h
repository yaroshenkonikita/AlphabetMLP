#pragma once

#include <QDialog>

namespace Ui {
class Settings;
}

namespace s21 {

//! Класс окна настроек
class Settings : public QDialog {
  Q_OBJECT

 public:
  //! Конструктор
  explicit Settings(QWidget *parent = nullptr);
  //! Деструктор
  ~Settings() override;

 private slots:
  //! Слот на нажатие кнопки "Принять"
  void on_buttonBox_accepted();

 private:
  //! Указатель на UI
  Ui::Settings *ui;
};

}  // namespace s21
