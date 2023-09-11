#pragma once

#include <optional>
#include <vector>

#include "../../third-party/matrix.h"

namespace s21 {
//! Ридер для обработки EMNIST данных для обучения перцептрона
class ReaderEMNIST {
 public:
  //! Тип хранящий одно значение из сенсоров и ответа
  typedef std::pair<Matrix<float>, std::size_t> EmnistValue;
  //! Дефолтный конструктор
  ReaderEMNIST() = default;
  //! Удален конструктор копирования
  ReaderEMNIST(const ReaderEMNIST &) = delete;
  //! Удален конструктор переноса
  ReaderEMNIST(ReaderEMNIST &&) noexcept = delete;
  //! Удален оператор копирования
  ReaderEMNIST &operator=(const ReaderEMNIST &) = delete;
  //! Удален оператор переноса
  ReaderEMNIST &operator=(ReaderEMNIST &&) noexcept = delete;
  /**
   * @brief Конструктор с открытием файла
   * @param path Путь до файла
   */
  explicit ReaderEMNIST(const std::string &path);
  //! Дефолтный деструктор
  ~ReaderEMNIST() = default;
  /**
   * @brief Прочитать значения EmnistValue из файла
   * @param path Путь до файла
   */
  void OpenFile(const std::string &path);
  //! Получение значения EMNIST по индексу
  EmnistValue operator[](std::size_t) const;
  //! Размер вектора EMNIST данных
  std::size_t Size() const;
  //! Получить вектор EMNIST с заданными рамками
  std::vector<EmnistValue> GetVector(std::size_t start = 0,
                                     std::size_t end = UINT64_MAX) const;

 private:
  //! Размер входных сенсоров в EMNIST
  static constexpr std::size_t count_sensors = 784;
  //! Вектор данных EMNIST
  std::vector<EmnistValue> lines_;
};

}  // namespace s21
