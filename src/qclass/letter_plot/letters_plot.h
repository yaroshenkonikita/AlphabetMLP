#pragma once

#include <QWidget>
#include <QtWidgets>
#include <iostream>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage,
                                          qreal radius, bool quality,
                                          bool alphaOnly, int transposed = 0);

QT_END_NAMESPACE

namespace Ui {
class LettersPlot;
}

namespace s21 {

//! Виджет для рисования
class LettersPlot : public QWidget {
  Q_OBJECT
 public:
  /**
   * @brief Дефолтный конструктор
   * @param parent Указатель на родителя
   */
  explicit LettersPlot(QWidget *parent = nullptr);
  /**
   * @brief Нарисовать картинку на виджете
   * @param image Картинка
   */
  void DrawImage(QImage image);

 private:
  //! Рисунок
  QPixmap pixmap_;
  //! Последняя нажатая клавиша
  Qt::MouseButton last_click_;
  /**
   * @brief Нарисовать круг на позиции мыши
   * @param pos Позиция мыши
   */
  void Draw(const QPoint &pos);
  //! Очистить окно
  void ClearWindow();
  //! Отправить рисунок в контроллер
  void SavePixels();
  //! Нарисовать рисунок
  void paintEvent(QPaintEvent *) override;
  //! Изменить размер рисунка
  void resizeEvent(QResizeEvent *) override;
  /**
   * @brief Обработка нажатия клавиш мышки
   * @param ev Указатель события мышки
   */
  void mousePressEvent(QMouseEvent *ev) override;
  /**
   * @brief Обработка движения мышки с зажатой клавишой
   * @param ev Указатель события мышки
   */
  void mouseMoveEvent(QMouseEvent *ev) override;
};

}  // namespace s21
