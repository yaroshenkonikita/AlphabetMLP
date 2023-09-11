#pragma once

#include <functional>

#include "model/model.h"
#include "qclass/main_window/main_window.h"

namespace s21 {

//! Контроллер приложения
class Controller {
 public:
  //! Получить Instance контроллера
  static Controller &GetInstance();
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
   * @brief Обучить модель
   * @param path Путь до обучающей выборки
   */
  void Learn(std::string path);
  /**
   * @brief Протестировать модель
   * @param path Путь до тестовой выборки
   */
  Model::TestOutput Test(std::string path);
  /**
   * @brief Обработать входные сенсоры
   * @details Ответы отправляет в обработчик основного окна
   * @param sensors Входные сенсоры
   */
  void ForwardFeed(const Matrix<float> &sensors);
  //! Показать основное окно
  void ShowWindow();
  //! Очистить основное окно
  void ClearWindow();

 private:
  //! Дефолтный конструктор
  Controller();
  // Основное окно
  MainWindow window_;
  // Модель
  Model model_;
};

}  // namespace s21
