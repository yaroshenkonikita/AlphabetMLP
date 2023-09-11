#include "graph_network.h"

#include <cmath>
#include <fstream>
#include <random>
#include <vector>

namespace s21 {

GraphNetwork::Weight::Weight(Neuron &child) : target(child) {
  static std::random_device rd;
  static std::mt19937 mt(rd());
  static std::uniform_real_distribution<float> dist(-1.0, 1.0);
  weight = dist(mt);
}

GraphNetwork::Weight::Weight(Neuron &child, float _weight)
    : target(child), weight(_weight) {}

void GraphNetwork::Weight::AddValue(const float value) {
  target.value += weight * value;
}

GraphNetwork::Layer::Layer(std::size_t count_neirons)
    : neurons(count_neirons) {}

void GraphNetwork::Layer::SendValues() {
  for (Neuron &neuron : neurons) {
    for (Weight &weight : neuron.weights) {
      weight.AddValue(neuron.value);
    }
  }
}

void GraphNetwork::Layer::Sigmoid() {
  for (Neuron &neuron : neurons) {
    neuron.value = 1.f / (1.f + std::exp(-neuron.value));
  }
}

void GraphNetwork::Layer::AddBias() {
  for (Neuron &neuron : neurons) {
    neuron.value += neuron.bias;
  }
}

void GraphNetwork::Layer::TakeError() {
  for (Neuron &neuron : neurons) {
    for (auto &[target, weight] : neuron.weights) {
      neuron.error += target.error * weight;
    }
    neuron.error *= neuron.value * (1 - neuron.value);
  }
}

void GraphNetwork::Layer::FixWeight(float learning_rate) {
  for (auto &neuron : neurons) {
    const float learning_value = neuron.value * learning_rate;
    for (auto &weight : neuron.weights) {
      const float error = learning_value * weight.target.error;
      weight.weight += error;
    }
  }
}

void GraphNetwork::Layer::FixBias(float learning_rate) {
  for (auto &neuron : neurons) {
    neuron.bias += neuron.error * learning_rate;
  }
}

void GraphNetwork::Layer::ClearValues() {
  for (Neuron &neuron : neurons) {
    neuron.error = neuron.value = 0.f;
  }
}

GraphNetwork::GraphNetwork(const std::vector<std::size_t> &layers) {
  if (layers.size() < 4 || layers.size() > 7) {
    throw std::invalid_argument("The network size must be between 2 and 5.");
  }
  layers_.emplace_back(layers.front());
  for (std::size_t index = 1; index < layers.size(); ++index) {
    layers_.emplace_back(layers[index]);
    for (auto &parent : layers_[index - 1].neurons) {
      for (auto &child : layers_[index].neurons) {
        parent.weights.emplace_back(child);
      }
    }
  }
}

Matrix<float> GraphNetwork::ForwardFeed(const Matrix<float> &sensors) {
  InitFullWay(sensors);
  auto last_layer = FromNeuronsToMatrix(layers_.back().neurons);
  ClearValues();
  return last_layer;
}

void GraphNetwork::Learn(const Matrix<float> &sensors, const std::size_t answer,
                         const float learning_rate) {
  InitFullWay(sensors);
  BackPropagation(answer, learning_rate);
  auto last_layer = FromNeuronsToMatrix(layers_.back().neurons);
  ClearValues();
}

std::pair<std::size_t, std::size_t> GraphNetwork::LoadWeights(
    std::string path) {
  std::ifstream file{path};
  std::size_t size = 0;
  file >> size;
  if (size < 3 || size > 6) {
    throw std::invalid_argument("The network size must be between 2 and 5.");
  }
  std::vector<std::pair<Matrix<float>, Matrix<float>>> layers;
  layers.resize(size);
  for (auto &[weights, bias] : layers) {
    file >> weights >> bias;
  }
  file.close();
  if (!file.bad()) {
    layers_.clear();
    layers_.emplace_back(layers.front().first.GetColumns());
    for (std::size_t layer = 0; layer < layers.size(); ++layer) {
      layers_.emplace_back(layers[layer].first.GetRows());
      for (std::size_t child = 0; child < layers_[layer + 1].neurons.size();
           ++child) {
        auto &child_neiron = layers_[layer + 1].neurons[child];
        for (std::size_t parent = 0; parent < layers_[layer].neurons.size();
             ++parent) {
          auto &parent_neiron = layers_[layer].neurons[parent];
          parent_neiron.weights.emplace_back(
              child_neiron, layers[layer].first(child, parent));
        }
        child_neiron.bias = layers[layer].second(child, 0);
      }
    }
  }
  return {layers_.size() - 2, layers_[1].neurons.size()};
}

void GraphNetwork::SaveWeights(std::string path) const {
  std::vector<std::pair<Matrix<float>, Matrix<float>>> neurons_to_save(
      layers_.size() - 1);
  for (std::size_t index = 0; (index + 1) < layers_.size(); ++index) {
    const std::size_t size_cols = layers_[index].neurons.size();
    const std::size_t size_rows = layers_[index + 1].neurons.size();
    neurons_to_save[index].first.Set(size_rows, size_cols);
    for (std::size_t cols = 0; cols < size_cols; ++cols) {
      for (std::size_t rows = 0; rows < size_rows; ++rows) {
        neurons_to_save[index].first(rows, cols) =
            layers_[index].neurons[cols].weights[rows].weight;
      }
    }
    neurons_to_save[index].second.Set(size_rows, 1);
    for (std::size_t rows = 0; rows < size_rows; ++rows) {
      neurons_to_save[index].second(rows, 0) =
          layers_[index + 1].neurons[rows].bias;
    }
  }
  std::ofstream file{path};
  file << neurons_to_save.size() << '\n';
  for (auto &[weights, bias] : neurons_to_save) {
    file << weights << bias;
  }
  file.close();
}

void GraphNetwork::InitFullWay(const Matrix<float> &sensors) {
  for (std::size_t i = 0; i < layers_[0].neurons.size(); ++i) {
    layers_[0].neurons[i].value = sensors(i, 0);
  }
  for (std::size_t i = 0; (i + 1) < layers_.size(); ++i) {
    layers_[i].SendValues();
    layers_[i + 1].AddBias();
    layers_[i + 1].Sigmoid();
  }
}

void GraphNetwork::BackPropagation(const std::size_t answer,
                                   const float learning_rate) {
  if (answer > 25) {
    throw std::invalid_argument("Bad EMNIST: answer letter is out of index");
  }
  std::size_t current = layers_.size() - 1;
  auto &last_layer = layers_[current].neurons;
  mse = 0.f;
  for (std::size_t neuron = 0; neuron < last_layer.size(); ++neuron) {
    const float value = last_layer[neuron].value;
    const float isAnswer = (neuron == answer ? 1.f : 0.f);
    const float error = value * (1 - value) * (isAnswer - value);
    last_layer[neuron].error = error;
    mse += powf(isAnswer - value, 2);
  }
  while (--current > 0) {
    layers_[current].TakeError();
  }
  while (++current < layers_.size()) {
    layers_[current - 1].FixWeight(learning_rate);
    layers_[current].FixBias(learning_rate);
  }
}

void GraphNetwork::ClearValues() {
  for (auto &neurons : layers_) {
    neurons.ClearValues();
  }
}

Matrix<float> GraphNetwork::FromNeuronsToMatrix(
    const std::vector<Neuron> &neurons) {
  Matrix<float> layer(neurons.size(), 1);
  for (std::size_t index = 0; index < neurons.size(); ++index) {
    layer(index, 0) = neurons[index].value;
  }
  return layer;
}

}  // namespace s21
