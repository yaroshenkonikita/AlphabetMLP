#pragma once

#include "../base/base_network.h"

namespace s21 {

//! Матричный перцептрон
class MatrixNetwork final : public BaseNetwork {
 public:
  //! Удален дефолтный конструктор
  MatrixNetwork() = delete;
  /**
   * @brief Конструктор с заданными слоями
   * @details Создает слои размером каждого значения вектора
   * @param layers Вектор размеров слоев
   */
  explicit MatrixNetwork(const std::vector<std::size_t> &layers);
  //! Дефолтный конструктор копирования
  MatrixNetwork(const MatrixNetwork &a) = default;
  //! Дефолтный конструктор переноса
  MatrixNetwork(MatrixNetwork &&a) = default;
  //! Дефолтный конструктор копирования
  MatrixNetwork &operator=(const MatrixNetwork &) = default;
  //! Дефолтный конструктор переноса
  MatrixNetwork &operator=(MatrixNetwork &&) = default;
  //! Переопределение дефолтного контруктора
  ~MatrixNetwork() override = default;
  /**
   * @brief Прогона входных сенсоров
   * @param sensors Входные сенсоры
   * @return Результативная матрица прогона данных по весам перцептрона
   */
  Matrix<float> ForwardFeed(const Matrix<float> &sensors) override;
  /**
   * @brief Обучение перцептрона
   * @param sensors Входные сенсоры
   * @param answer Правильный индекс
   * @param learning_rate Скорость обучения
   */
  void Learn(const Matrix<float> &sensors, std::size_t answer,
             float learning_rate) override;
  /**
   * @brief Загрузить веса
   * @param path Путь до файла
   * @return Пара (Количество скрытых слоев, Количество нейронов в скрытом слою)
   */
  std::pair<std::size_t, std::size_t> LoadWeights(std::string path) override;
  /**
   * @brief Сохранить веса
   * @param path Путь до файла
   */
  void SaveWeights(std::string path) const override;

 private:
  //! Слой перцептрона
  struct Layer {
    //! Дефолтный конструктор
    Layer() = default;
    //! Дефолтный конструктор копирования
    Layer(const Layer &a) = default;
    //! Дефолтный конструктор переноса
    Layer(Layer &&a) = default;
    //! Дефолтный конструктор копирования
    Layer &operator=(const Layer &) = default;
    //! Дефолтный конструктор переноса
    Layer &operator=(Layer &&) = default;
    /**
     * @brief Конструктор с заданием размеров весов/смещений
     * @param weight_rows Количество строк весов
     * @param weight_cols Количество столбцов весов
     * @param bias_rows Количество строк смещений
     * @param bias_cols Количество столбцов смещений
     */
    Layer(std::size_t weight_rows, std::size_t weight_cols,
          std::size_t bias_rows, std::size_t bias_cols);
    //! Матрица весов
    Matrix<float> weights;
    //! Матрица смещений
    Matrix<float> biases;
  };
  /**
   * @brief Прогнать все значения по сети
   * @param sensors Входные сенсоры
   * @return Вектор значений нейронов
   */
  std::vector<Matrix<float>> GetFullWay(const Matrix<float> &sensors) const;
  //! Заполнить веса случайными значениями
  void FillWeight();
  /**
   * @brief Применить сигмоиду ко всем значениям матрицы
   * @param matrix Исходная матрица
   * @return Итоговая матрица после сигмоиды
   */
  static Matrix<float> Sigmoid(Matrix<float> matrix);
  /**
   * @brief Выполнить обратное распространение ошибок
   * @param way Вектор матриц значений нейронов
   * @param answer Правильный выходной индекс
   * @param learning_rate Скорость обучения
   */
  void BackPropagation(const std::vector<Matrix<float>> &way,
                       std::size_t answer, float learning_rate);
  //! Слои сети
  std::vector<Layer> layers_;
};

}  // namespace s21
