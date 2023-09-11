#pragma once

#include <QDateTime>
#include <QMainWindow>

namespace Ui {
class GraphMseWindow;
}

namespace s21 {

//! Класс окна графика средней квадратичной ошибки
class GraphMseWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Дефолтный конструктор
   * @param parent Указатель на родителя
   */
  explicit GraphMseWindow(QWidget *parent = nullptr);
  //! Дефолтный деструктор
  ~GraphMseWindow() override;
  /**
   * @brief Добавить график средней ошибки
   * @param mse Вектор графика
   */
  void AddGraph(std::vector<double> mse);
  //! Очистить график
  void ClearGraph();

 private:
  //! Указатель на UI
  Ui::GraphMseWindow *ui;
  //! Максимальное количество значений в графиках
  std::size_t max_count_;
  //! Максимальное значение ошибки в крафиках
  float max_value_;
};

}  // namespace s21
