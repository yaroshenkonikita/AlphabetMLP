#include "main_window.h"

#include <QFileDialog>
#include <QMessageBox>
#include <functional>
#include <map>

#include "../../controller/controller.h"
#include "qclass/settings/settings.h"
#include "ui_main_window.h"

namespace s21 {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      graph_window_(new GraphMseWindow(this)),
      ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  initTableAnswers();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::AddGraphMse(const std::vector<double> &mse_vector) {
  graph_window_->AddGraph(mse_vector);
}

void MainWindow::UpdateLettersAnswer(const Matrix<float> &answers) {
  std::map<float, char, std::greater<>> letters_proc;
  for (std::size_t index = 0; index < answers.GetRows(); ++index) {
    letters_proc.emplace(answers(index, 0) * 100.f, 'A' + index);
  }
  auto current = letters_proc.begin();
  for (int row = 0; row < ui_->tableAnswers->rowCount(); ++row, ++current) {
    ui_->tableAnswers->item(row, 0)->setText(QString(current->second));
    ui_->tableAnswers->item(row, 1)->setText(
        QString::number(current->first, 'f', 2) + '%');
  }
}

void MainWindow::initTableAnswers() {
  ui_->tableAnswers->setColumnWidth(0, 20);
  ui_->tableAnswers->setColumnWidth(1, 67);
  for (int row = 0; row < ui_->tableAnswers->rowCount(); ++row) {
    for (int column = 0; column < ui_->tableAnswers->columnCount(); ++column) {
      auto *item = new QTableWidgetItem();
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      ui_->tableAnswers->setItem(row, column, item);
    }
  }
}

void MainWindow::clearTableAnswers() {
  for (int row = 0; row < ui_->tableAnswers->rowCount(); ++row) {
    ui_->tableAnswers->item(row, 0)->setText("");
    ui_->tableAnswers->item(row, 1)->setText("");
  }
}

void MainWindow::closeEvent(QCloseEvent *) {
  delete graph_window_;
  graph_window_ = nullptr;
}

void MainWindow::on_test_action_triggered() {
  QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                              tr("Table files (*.csv)"));
  if (path.isEmpty()) {
    return;
  }
  auto test_output = Controller::GetInstance().Test(path.toStdString());
  QMessageBox::information(
      this, "Внимание",
      "Тестирование закончилось!"
      "\nPrecision: " +
          QString::number(test_output.precision * 100.f, 'f', 2) +
          "%\nAverage accuracy: " +
          QString::number(test_output.average_accuracy * 100.f, 'f', 2) +
          "%\nF-Measure: " +
          QString::number(test_output.f_measure * 100.f, 'f', 2) +
          "%\nRecall: " + QString::number(test_output.recall * 100.f, 'f', 2) +
          "%\nTime: " + QString::number(test_output.time_sec, 'f', 2) + " sec");
}

void MainWindow::on_learn_action_triggered() {
  QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                              tr("Table files (*.csv)"));
  if (path.isEmpty()) {
    return;
  }
  std::clock_t start = clock();
  Controller::GetInstance().Learn(path.toStdString());
  QMessageBox::information(
      this, "Внимание",
      "Обучение закончилось!\nОбучение длилось: " +
          QString::number(static_cast<double>(clock() - start) /
                              static_cast<double>(CLOCKS_PER_SEC),
                          'f', 2) +
          " sec");
  graph_window_->show();
  graph_window_->update();
}

void MainWindow::on_open_graph_action_triggered() { graph_window_->show(); }

void MainWindow::on_settings_action_triggered() {
  Settings view;
  view.exec();
}

void MainWindow::on_load_network_action_triggered() {
  QString path = QFileDialog::getOpenFileName(this, tr("Open Network"), ".",
                                              tr("Net files (*.net)"));
  if (path.isEmpty()) {
    return;
  }
  Controller::GetInstance().LoadWeights(path.toStdString());
}

void MainWindow::on_save_network_action_triggered() {
  QString path = QFileDialog::getSaveFileName(
      this, tr("Save Network"), "network.net", tr("Net files (*.net)"));
  if (path.isEmpty()) {
    return;
  }
  Controller::GetInstance().SaveWeights(path.toStdString());
}

void MainWindow::on_load_image_letter_action_triggered() {
  QString path = QFileDialog::getOpenFileName(this, tr("Load image"), ".",
                                              tr("Image Files (*.bmp)"));
  if (path.isEmpty()) {
    return;
  }

  Matrix<float> pixels(784, 1);
  auto pixels_image = QPixmap(path).scaled(28, 28).toImage();
  for (int i = 0, index = 0; i < pixels_image.size().width(); ++i) {
    for (int j = 0; j < pixels_image.size().height(); ++j, ++index) {
      pixels(index, 0) = (pixels_image.pixelColor(i, j).black() / 255.f);
    }
  }
  Controller::GetInstance().ForwardFeed(pixels);
  ui_->widget->DrawImage(pixels_image);
}

}  // namespace s21
