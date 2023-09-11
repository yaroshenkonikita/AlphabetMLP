#pragma once

#include "../../../third-party/matrix.h"

namespace s21 {
//! Интерфейс перцептрона
class BaseNetwork {
 public:
  //! Дефолтный конструктор
  BaseNetwork() = default;
  /**
   * @brief Прототип дефолтного деструктора
   */
  virtual ~BaseNetwork() = default;
  /**
   * @brief Прототип обучения перцептрона
   * @param sensors Входные сенсоры
   * @param answer Правильный индекс
   * @param learning_rate Скорость обучения
   */
  virtual void Learn(const Matrix<float> &sensors, std::size_t answer,
                     float learning_rate) = 0;
  /**
   * @brief Прототип прогона входных сенсоров
   * @param sensors Входные сенсоры
   * @return Результативная матрица прогона данных по весам перцептрона
   */
  virtual Matrix<float> ForwardFeed(const Matrix<float> &sensors) = 0;
  /**
   * @brief Загрузить веса
   * @param path Путь до файла
   * @return Пара (Количество скрытых слоев, Количество нейронов в скрытом слою)
   */
  virtual std::pair<std::size_t, std::size_t> LoadWeights(std::string path) = 0;
  /**
   * @brief Сохранить веса
   * @param path Путь до файла
   */
  virtual void SaveWeights(std::string path) const = 0;
  /**
   * @brief Получить индекс максимального значения в выходном слое
   * @param last_layer Выходной слой
   * @return std::size_t Индекс с максимальным значением
   */
  static std::size_t GetRightIndex(const Matrix<float> &last_layer);
  //! Получить значение средней квадратичной ошибки
  double GetLastMse() const;

 protected:
  //! Значение средней квадратичной ошибки
  double mse = 0;
};

}  // namespace s21
