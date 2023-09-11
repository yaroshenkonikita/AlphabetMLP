#include "settings.h"

#include "controller/controller.h"
#include "ui_settings.h"

namespace s21 {

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
  ui->setupUi(this);
  ui->layers_spin_box->setValue(Controller::GetInstance().GetCountLayers());
  ui->neirons_spin_box->setValue(Controller::GetInstance().GetCountNeurons());
  ui->matrix_radio_button->setChecked(
      Controller::GetInstance().IsMatrixNetwork());
  ui->graph_radio_button->setChecked(
      Controller::GetInstance().IsGraphNetwork());
  ui->epoch_spin_box->setValue(Controller::GetInstance().GetCountEpoch());
  ui->k_valid_spin_box->setValue(Controller::GetInstance().GetKValid());
  ui->learning_rate_double_spin_box->setValue(
      Controller::GetInstance().GetLearningRate());
  ui->part_test_horizontal_slider->setValue(
      Controller::GetInstance().GetTestSample() * 100.f);
}

void Settings::on_buttonBox_accepted() {
  Controller::GetInstance().SetCountLayers(ui->layers_spin_box->value());
  Controller::GetInstance().SetCountNeurons(ui->neirons_spin_box->value());
  if (ui->matrix_radio_button->isChecked()) {
    Controller::GetInstance().SetMatrixNetwork();
  } else {
    Controller::GetInstance().SetGraphNetwork();
  }
  Controller::GetInstance().SetCountEpoch(ui->epoch_spin_box->value());
  Controller::GetInstance().SetLearningRate(
      ui->learning_rate_double_spin_box->value());
  Controller::GetInstance().SetTestSample(
      ui->part_test_horizontal_slider->value() / 100.f);
  Controller::GetInstance().SetKValid(ui->k_valid_spin_box->value());
}

Settings::~Settings() { delete ui; }

}  // namespace s21
