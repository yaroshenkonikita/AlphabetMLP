#include "letters_plot.h"

#include "../../controller/controller.h"

namespace s21 {

LettersPlot::LettersPlot(QWidget *parent)
    : QWidget{parent}, pixmap_(size()), last_click_() {
  QPainter painter{&pixmap_};
  painter.fillRect(rect(), Qt::white);
}

void LettersPlot::DrawImage(QImage image) {
  pixmap_.convertFromImage(image);
  resizeEvent(NULL);
  update();
}

void LettersPlot::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.drawPixmap(0, 0, pixmap_);
}

void LettersPlot::resizeEvent(QResizeEvent *) {
  if (size() != pixmap_.size()) {
    QPixmap newPixmap{size()};
    QPainter painter{&newPixmap};
    painter.fillRect(rect(), Qt::white);
    painter.drawPixmap(0, 0, pixmap_.scaled(28, 28).scaled(size()));
    pixmap_ = newPixmap;
  }
}

void LettersPlot::mousePressEvent(QMouseEvent *ev) {
  last_click_ = ev->button();
  if (ev->button() == Qt::RightButton) {
    ClearWindow();
    Controller::GetInstance().ClearWindow();
  }
  mouseMoveEvent(ev);
}

void LettersPlot::mouseMoveEvent(QMouseEvent *ev) {
  if (last_click_ == Qt::LeftButton) {
    Draw(ev->pos());
    SavePixels();
  }
}

void LettersPlot::ClearWindow() {
  if (pixmap_.paintingActive()) {
    return;
  }
  QPainter painter{&pixmap_};
  painter.eraseRect(this->rect());
  update();
  Controller::GetInstance().ForwardFeed(Matrix<float>(784, 1));
}

void LettersPlot::Draw(const QPoint &pos) {
  QPainter painter{&pixmap_};
  painter.setRenderHint(QPainter::Antialiasing);
  QPen pen;
  pen.setWidth(28);
  pen.setColor(Qt::black);
  painter.setPen(pen);
  painter.drawEllipse(pos, 14, 14);
  update();
}

void LettersPlot::SavePixels() {
  QImage img;
  img = pixmap_.toImage();
  img = img.scaled(28, 28);
  pixmap_.convertFromImage(img.scaled(pixmap_.size()));

  QPixmap pxDst(img.size());
  pxDst.fill(Qt::transparent);
  {
    QPainter painter(&pxDst);
    qt_blurImage(&painter, img, 4, true, false);
  }

  Matrix<float> pixels(784, 1);
  auto pixels_image = pxDst.toImage();
  for (int i = 0, index = 0; i < pixels_image.size().width(); ++i) {
    for (int j = 0; j < pixels_image.size().height(); ++j, ++index) {
      pixels(index, 0) =
          (static_cast<float>(pixels_image.pixelColor(i, j).black()) / 255.f);
    }
  }
  Controller::GetInstance().ForwardFeed(pixels);
}

}  // namespace s21
