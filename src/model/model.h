#pragma once

#include "networks/base/base_network.h"
#include "networks/graph/graph_network.h"
#include "networks/matrix/matrix_network.h"
#include "reader/reader_emnist.h"

namespace s21 {

//! Модель приложения
class Model {
 public:
  //! Размер входного слоя
  static constexpr std::size_t inner_layer_size = 784;
  //! Размер вы1ходного слоя
  static constexpr std::size_t outer_layer_size = 26;
  //! Структура вывода теста
  struct TestOutput {
    double average_accuracy, precision, recall, f_measure, time_sec;
  };
  //! Дефолтный конструктор
  Model();
  //! Удален конструктор копирования
  Model(const Model &) = delete;
  //! Удален конструктор переноса
  Model(Model &&) noexcept = delete;
  //! Удален оператор копирования
  Model &operator=(const Model &) = delete;
  //! Удален оператор переноса
  Model &operator=(Model &&) noexcept = delete;
  // Дефолтный деструктор
  ~Model();
  /**
   * @brief Сохранить веса модели в файл
   */
  void SaveWeights(std::string) const;
  /**
   * @brief Загрузить веса из файла в модель
   */
  void LoadWeights(std::string);
  //! Установить матричную сеть
  void SetMatrixNetwork();
  //! Узнать матричная ли сеть
  bool IsMatrixNetwork() const;
  //! Установить графовую сеть
  void SetGraphNetwork();
  //! Узнать графовая ли сеть
  bool IsGraphNetwork() const;
  //! Установить скорость обучения
  void SetLearningRate(float);
  //! Получить скорость обучения
  float GetLearningRate() const;
  //! Установить количество слоев
  void SetCountLayers(std::size_t);
  //! Получить количество слоев
  std::size_t GetCountLayers() const;
  //! Установить количество нейронов в слою
  void SetCountNeurons(std::size_t);
  //! Получить количество нейронов в слою
  std::size_t GetCountNeurons() const;
  //! Установить количество эпох обучения
  void SetCountEpoch(std::size_t);
  //! Получить количество эпох обучения
  std::size_t GetCountEpoch() const;
  //! Установить множитель размера тестовой выборки
  void SetTestSample(float);
  //! Получить множитель размера размер тестовой выборки
  float GetTestSample() const;
  //! Установить количество к-валидации
  void SetKValid(std::size_t);
  //! Получить количество количество к-валидации
  std::size_t GetKValid() const;
  /**
   * @brief Обработать входные сенсоры
   * @param sensors Входные сенсоры
   */
  Matrix<float> ForwardFeed(const Matrix<float> &sensors);
  /**
   * @brief Обучить перцептрон
   * @param reader Ридер с обучающей выборкой
   */
  std::vector<double> Learn(const ReaderEMNIST &reader);
  /**
   * @brief Протестировать перцептрон
   * @param reader Ридер с тестовой выборкой
   */
  TestOutput Test(const ReaderEMNIST &reader);

 private:
  //! Перечисление типов перцептрона
  enum class TypeNetwork { Matrix, Graph };
  //! Обновить конфигурацию перцептрона
  void UpdateNetwork();
  //! Количество слоев в перцептроне
  std::size_t count_layers_;
  //! Количество нейронов в скрытых слоях перцептрона
  std::size_t count_neurons_;
  //! Тип перцептрона
  TypeNetwork type_network_;
  //! Количество эпох при обучении
  std::size_t count_epoch_;
  //! Число к-валидации при обучении
  std::size_t k_valid_;
  //! Скорость обучения
  float learning_rate_;
  //! Множитель размера тестовой выборки
  float test_sample_;
  //! Указатель на перцептрон
  BaseNetwork *network_;
};

}  // namespace s21
