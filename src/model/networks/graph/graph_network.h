#pragma once

#include "../base/base_network.h"

namespace s21 {
//! Графовый перцептрон
class GraphNetwork final : public BaseNetwork {
 public:
  //! Удален дефолтный конструктор
  GraphNetwork() = delete;
  /**
   * @brief Конструктор с заданными слоями
   * @details Создает слои размером каждого значения вектора
   * @param layers Вектор размеров слоев
   */
  explicit GraphNetwork(const std::vector<std::size_t> &layers);
  //! Дефолтный конструктор копирования
  GraphNetwork(const GraphNetwork &a) = default;
  //! Дефолтный конструктор переноса
  GraphNetwork(GraphNetwork &&a) noexcept = default;
  //! Дефолтный конструктор копирования
  GraphNetwork &operator=(const GraphNetwork &) = default;
  //! Дефолтный конструктор переноса
  GraphNetwork &operator=(GraphNetwork &&) noexcept = default;
  //! Переопределение дефолтного контруктора
  ~GraphNetwork() override = default;
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
  //! Предварительная декларация класса связи между нейронами
  struct Weight;
  //! Нейрон
  struct Neuron {
    float value = 0;              //!< Значение
    float error = 0;              //!< Ошибка
    float bias = 0;               //!< Смещение
    std::vector<Weight> weights;  //!< Веса на следующие нейроны
  };
  //! Класс связи между нейронами
  struct Weight {
    /**
     * @brief Конструктор веса связывающий следующий нейрон
     * @param neuron Следующий нейрон
     */
    explicit Weight(Neuron &neuron);
    //! Дефолтный конструктор копирования
    Weight(const Weight &a) = default;
    //! Дефолтный конструктор переноса
    Weight(Weight &&a) noexcept = default;
    //! Дефолтный конструктор копирования
    Weight &operator=(const Weight &) = default;
    //! Дефолтный конструктор переноса
    Weight &operator=(Weight &&) noexcept = default;
    /**
     * @brief Конструктор веса связывающий следующий нейрон
     * @param neuron Следующий нейрон
     * @param weight Заданный вес
     */
    Weight(Neuron &neuron, float weight);
    //! Дефолтный деструктор
    ~Weight() = default;
    /**
     * @brief Добавить значение умноженное на вес в значение заданного нейрона
     * @param value Значение
     */
    void AddValue(float value);
    Neuron &target;  //!< Заданный нейрон
    float weight;    //!< Вес
  };
  //! Слой перцептрона
  struct Layer {
    /**
     * @brief Конструктор слоя
     * @details Создает заданное количество нейронов
     * @param neurons Заданное количество нейронов
     */
    explicit Layer(std::size_t neurons);
    //! Дефолтный конструктор копирования
    Layer(const Layer &a) = default;
    //! Дефолтный конструктор переноса
    Layer(Layer &&a) noexcept = default;
    //! Дефолтный конструктор копирования
    Layer &operator=(const Layer &) = default;
    //! Дефолтный конструктор переноса
    Layer &operator=(Layer &&) noexcept = default;
    //! Дефолтный деструктор
    ~Layer() = default;
    //! Отправить значения дальше по весам
    void SendValues();
    //! Собрать значения из нейронов впереди
    void TakeError();
    //! Взять сигмоиду каждого нейрона
    void Sigmoid();
    //! Добавить смещение каждому нейрону
    void AddBias();
    /**
     * @brief Скоректировать веса по ошибке
     * @param learning_rate Скорость обучения
     */
    void FixWeight(float learning_rate);
    /**
     * @brief Скоректировать смещение по ошибке
     * @param learning_rate Скорость обучения
     */
    void FixBias(float learning_rate);
    //! Очистить значения и ошибки нейронов
    void ClearValues();
    std::vector<Neuron> neurons;  //!< Нейроны
  };
  /**
   * @brief Перевод нейронов в матрицу
   * @param neurons Исходные нейроны
   * @return Матрица переведенная из нейронов
   */
  static Matrix<float> FromNeuronsToMatrix(const std::vector<Neuron> &neurons);
  /**
   * @brief Прогнать все значения по сети
   * @param sensors Входные сенсоры
   */
  void InitFullWay(const Matrix<float> &sensors);
  /**
   * @brief Выполнить обратное распространение ошибок
   * @param answer Правильный выходной индекс
   * @param learning_rate Скорость обучения
   */
  void BackPropagation(std::size_t answer, float learning_rate);
  //! Очистить значения и ошибки
  void ClearValues();
  //! Слои сети
  std::vector<Layer> layers_;
};

}  // namespace s21
