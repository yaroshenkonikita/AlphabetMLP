#include "matrix_network.h"

#include <fstream>
#include <random>

namespace s21 {

MatrixNetwork::Layer::Layer(std::size_t weight_rows, std::size_t weight_cols,
                            std::size_t bias_rows, std::size_t bias_cols)
    : weights(weight_rows, weight_cols), biases(bias_rows, bias_cols) {}

MatrixNetwork::MatrixNetwork(const std::vector<std::size_t> &layers) {
  if (layers.size() < 4 || layers.size() > 7) {
    throw std::invalid_argument("The network size must be between 2 and 5.");
  }
  for (std::size_t index = 1; index < layers.size(); ++index) {
    layers_.emplace_back(layers[index], layers[index - 1], layers[index], 1);
  }
  FillWeight();
}

Matrix<float> MatrixNetwork::ForwardFeed(const Matrix<float> &sensor) {
  return GetFullWay(sensor).back();
}

void MatrixNetwork::Learn(const Matrix<float> &sensor, std::size_t answer,
                          const float learning_rate) {
  auto way = GetFullWay(sensor);
  BackPropagation(way, answer, learning_rate);
}

std::pair<std::size_t, std::size_t> MatrixNetwork::LoadWeights(
    std::string path) {
  std::ifstream file{path};
  std::size_t size = 0;
  file >> size;
  if (size < 3 || size > 6) {
    throw std::invalid_argument("The network size must be between 2 and 5.");
  }
  std::vector<Layer> layers;
  layers.resize(size);
  for (auto &[weights, bias] : layers) {
    file >> weights >> bias;
  }
  file.close();
  if (!file.bad()) {
    layers_ = std::move(layers);
  }
  return {layers_.size() - 1, layers_[0].biases.GetRows()};
}

void MatrixNetwork::SaveWeights(std::string path) const {
  std::ofstream file{path};
  file << layers_.size() << '\n';
  for (auto &[weights, bias] : layers_) {
    file << weights << bias;
  }
  file.close();
}

std::vector<Matrix<float>> MatrixNetwork::GetFullWay(
    const Matrix<float> &sensor) const {
  std::vector<Matrix<float>> way{sensor};
  for (auto &[weights, bias] : layers_) {
    way.push_back(Sigmoid(weights * way.back() + bias));
  }
  return way;
}

void MatrixNetwork::FillWeight() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1.0, 1.0);
  for (auto &[weight, bias] : layers_) {
    for (std::size_t i = 0; i < weight.GetRows(); ++i) {
      for (std::size_t j = 0; j < weight.GetColumns(); ++j) {
        weight(i, j) = dist(mt);
      }
      bias(i, 0) = 1.f;
    }
  }
}

Matrix<float> MatrixNetwork::Sigmoid(Matrix<float> matrix) {
  for (std::size_t i = 0; i < matrix.GetRows(); ++i) {
    for (std::size_t j = 0; j < matrix.GetColumns(); ++j) {
      float &item = matrix(i, j);
      item = 1.f / (1.f + std::exp(-item));
    }
  }
  return matrix;
}

void MatrixNetwork::BackPropagation(const std::vector<Matrix<float>> &way,
                                    std::size_t answer,
                                    const float learning_rate) {
  if (answer > 25) {
    throw std::invalid_argument("Bad EMNIST: answer letter is out of index");
  }
  std::vector<Matrix<float>> error(way);
  mse = 0;
  for (std::size_t i = 0; i < way.back().GetRows(); ++i) {
    const float value = way.back()(i, 0);
    const float isAnswer = (i == answer ? 1.f : 0.f);
    error.back()(i, 0) = value * (1 - value) * (isAnswer - value);
    mse += powf(isAnswer - value, 2);
  }
  for (std::size_t i = way.size() - 2; i > 0; --i) {
    error[i] = layers_[i].weights.Transpose() * error[i + 1];
    for (std::size_t j = 0; j < error[i].GetRows(); ++j) {
      error[i](j, 0) *= way[i](j, 0) * (1 - way[i](j, 0));
    }
  }
  float err;
  for (std::size_t i = 0; i < way.size() - 1; ++i) {
    for (std::size_t j = 0; j < way[i + 1].GetRows(); ++j) {
      err = error[i + 1](j, 0) * learning_rate;
      layers_[i].biases(j, 0) += err;
      for (std::size_t k = 0; k < way[i].GetRows(); ++k) {
        layers_[i].weights(j, k) += way[i](k, 0) * err;
      }
    }
  }
}

}  // namespace s21
