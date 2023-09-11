#include "controller.h"

#include <memory>

#include "model/reader/reader_emnist.h"

namespace s21 {

Controller::Controller() : window_(), model_() {}

Controller &Controller::GetInstance() {
  static std::unique_ptr<Controller> instance(new Controller());
  return *instance;
}

void Controller::SaveWeights(std::string path) const {
  model_.SaveWeights(path);
}

void Controller::LoadWeights(std::string path) { model_.LoadWeights(path); }

void Controller::SetMatrixNetwork() { model_.SetMatrixNetwork(); }

bool Controller::IsMatrixNetwork() const { return model_.IsMatrixNetwork(); }

void Controller::SetGraphNetwork() { model_.SetGraphNetwork(); }

bool Controller::IsGraphNetwork() const { return model_.IsGraphNetwork(); }

void Controller::SetLearningRate(const float learning_rate) {
  model_.SetLearningRate(learning_rate);
}

float Controller::GetLearningRate() const { return model_.GetLearningRate(); }

void Controller::SetCountLayers(const std::size_t count_layers) {
  model_.SetCountLayers(count_layers);
}

std::size_t Controller::GetCountLayers() const {
  return model_.GetCountLayers();
}

void Controller::SetCountNeurons(const std::size_t count_neurons) {
  model_.SetCountNeurons(count_neurons);
}

std::size_t Controller::GetCountNeurons() const {
  return model_.GetCountNeurons();
}

void Controller::SetCountEpoch(const std::size_t count_epoch) {
  model_.SetCountEpoch(count_epoch);
}

std::size_t Controller::GetCountEpoch() const { return model_.GetCountEpoch(); }

void Controller::SetTestSample(const float test_sample) {
  model_.SetTestSample(test_sample);
}

float Controller::GetTestSample() const { return model_.GetTestSample(); }

void Controller::SetKValid(const std::size_t k_valid) {
  model_.SetKValid(k_valid);
}

std::size_t Controller::GetKValid() const { return model_.GetKValid(); }

void Controller::Learn(std::string path) {
  ReaderEMNIST learn_values{path};
  if (learn_values.Size() == 0) {
    return;
  }
  for (std::size_t index = 0; index < model_.GetCountEpoch(); ++index) {
    window_.AddGraphMse(model_.Learn(learn_values));
  }
}

Model::TestOutput Controller::Test(std::string path) {
  return model_.Test(ReaderEMNIST(path));
}

void Controller::ForwardFeed(const Matrix<float> &line) {
  window_.UpdateLettersAnswer(model_.ForwardFeed(line));
}

void Controller::ShowWindow() { window_.show(); }

void Controller::ClearWindow() { window_.clearTableAnswers(); }

}  // namespace s21
