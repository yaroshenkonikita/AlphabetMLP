#pragma once

#include <QMainWindow>

#include "qclass/graph_mse/graph_mse_window.h"
#include "third-party/matrix.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

//! Класс основного окна
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Дефолтный конструктор
   * @param parent Указатель на родителя
   */
  explicit MainWindow(QWidget *parent = nullptr);

  //! Дефолтный деструктор
  ~MainWindow() override;

  /**
   * @brief Добавить график ошибок
   * @param mse_vector Вектор ошибок
   */
  void AddGraphMse(const std::vector<double> &mse_vector);

  /**
   * @brief Обновить ответы
   * @param answers Матрица ответов
   */
  void UpdateLettersAnswer(const Matrix<float> &answers);

  //! Очистить значения Таблички
  void clearTableAnswers();

 private slots:
  //! Слот обработки закрытия окна
  void closeEvent(QCloseEvent *) override;
  //! Слот нажатия кнопки "Тест"
  void on_test_action_triggered();
  //! Слот нажатия кнопки "Обучение"
  void on_learn_action_triggered();
  //! Слот нажатия кнопки "График"
  void on_open_graph_action_triggered();
  //! Слот нажатия кнопки "Настройки"
  void on_settings_action_triggered();
  //! Слот нажатия кнопки "Загрузить Network"
  void on_load_network_action_triggered();
  //! Слот нажатия кнопки "Сохранить Network"
  void on_save_network_action_triggered();
  //! Слот нажатия кнопки "Загрузить фото"
  void on_load_image_letter_action_triggered();

 private:
  //! Инициализация таблички
  void initTableAnswers();
  //! Указатель на окно Графика
  GraphMseWindow *graph_window_;
  //! Указатель на UI
  Ui::MainWindow *ui_;
};

}  // namespace s21
