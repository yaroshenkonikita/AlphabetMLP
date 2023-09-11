#include "model.h"

#include <ctime>
#include <stdexcept>
#include <utility>

namespace s21 {

Model::Model()
    : count_layers_(2),
      count_neurons_(64),
      type_network_(TypeNetwork::Matrix),
      count_epoch_(1),
      k_valid_(1),
      learning_rate_(0.2f),
      test_sample_(1.f),
      network_(new MatrixNetwork({inner_layer_size, count_neurons_,
                                  count_neurons_, outer_layer_size})) {}

Model::~Model() { delete network_; }

void Model::SetMatrixNetwork() {
  if (type_network_ == TypeNetwork::Matrix) {
    return;
  }
  type_network_ = TypeNetwork::Matrix;
  UpdateNetwork();
}

bool Model::IsMatrixNetwork() const {
  return type_network_ == TypeNetwork::Matrix;
}

void Model::SetGraphNetwork() {
  if (type_network_ == TypeNetwork::Graph) {
    return;
  }
  type_network_ = TypeNetwork::Graph;
  UpdateNetwork();
}

bool Model::IsGraphNetwork() const {
  return type_network_ == TypeNetwork::Graph;
}

void Model::SetLearningRate(float learning_rate) {
  learning_rate = std::max(learning_rate, 0.f);
  learning_rate_ = learning_rate;
}

float Model::GetLearningRate() const { return learning_rate_; }

void Model::SetCountLayers(std::size_t count_layers) {
  count_layers = std::max(count_layers, 1lu);
  if (count_layers_ == count_layers) {
    return;
  }
  count_layers_ = count_layers;
  UpdateNetwork();
}

std::size_t Model::GetCountLayers() const { return count_layers_; }

void Model::SetCountNeurons(std::size_t count_neurons) {
  count_neurons = std::max(count_neurons, 1lu);
  if (count_neurons_ == count_neurons) {
    return;
  }
  count_neurons_ = count_neurons;
  UpdateNetwork();
}

std::size_t Model::GetCountNeurons() const { return count_neurons_; }

void Model::SetCountEpoch(const std::size_t count_epoch) {
  count_epoch_ = std::max(count_epoch, 1lu);
}

std::size_t Model::GetCountEpoch() const { return count_epoch_; }

void Model::SetTestSample(const float test_sample) {
  test_sample_ = std::clamp(test_sample, 0.f, 1.f);
}

float Model::GetTestSample() const { return test_sample_; }

void Model::SetKValid(const std::size_t k_valid) {
  k_valid_ = std::max(k_valid, 1lu);
}

std::size_t Model::GetKValid() const { return k_valid_; }

void Model::UpdateNetwork() {
  std::vector<std::size_t> neurons;
  neurons.push_back(inner_layer_size);
  for (std::size_t index = 0; index < count_layers_; ++index) {
    neurons.push_back(count_neurons_);
  }
  neurons.push_back(outer_layer_size);
  delete network_;
  switch (type_network_) {
    case TypeNetwork::Matrix:
      network_ = new MatrixNetwork(neurons);
      break;
    case TypeNetwork::Graph:
      network_ = new GraphNetwork(neurons);
      break;
    default:
      throw std::logic_error("Haven't network type");
  }
}

void Model::SaveWeights(std::string path) const {
  network_->SaveWeights(std::move(path));
}

void Model::LoadWeights(std::string path) {
  auto [count_layers, count_neurons] = network_->LoadWeights(std::move(path));
  count_layers_ = count_layers, count_neurons_ = count_neurons;
}

Matrix<float> Model::ForwardFeed(const Matrix<float> &data) {
  return network_->ForwardFeed(data);
}

std::vector<double> Model::Learn(const ReaderEMNIST &reader) {
  std::vector<double> mse;
  auto learn_row =
      [this, &mse](const std::vector<ReaderEMNIST::EmnistValue> &requests) {
        for (auto &request : requests) {
          network_->Learn(request.first, request.second, learning_rate_);
          mse.push_back(network_->GetLastMse());
        }
      };
  std::size_t delta = reader.Size() / k_valid_;
  if (k_valid_ <= 2 || delta == 0) {
    learn_row(reader.GetVector());
  } else {
    for (std::size_t index = 0; index < k_valid_; ++index) {
      learn_row(reader.GetVector(0, index * delta));
      learn_row(reader.GetVector((index + 1) * delta));
    }
  }
  return mse;
}

Model::TestOutput Model::Test(const ReaderEMNIST &reader) {
  if (static_cast<std::size_t>(static_cast<float>(reader.Size()) *
                               test_sample_) == 0lu) {
    return TestOutput{};
  }
  double TP = 0, FP = 0, FN = 0, TN = 0;
  clock_t start = clock();
  auto vector_requests = reader.GetVector();
  const auto max_index = static_cast<std::size_t>(
      static_cast<float>(vector_requests.size()) * test_sample_);
  for (std::size_t index = 0; index < max_index; ++index) {
    auto &request = vector_requests[index];
    auto response = network_->ForwardFeed(request.first);
    for (std::size_t row = 0; row < response.GetRows(); ++row) {
      if (response(row, 0) >= 0.5f) {
        row == request.second ? ++TP : ++FP;
      } else {
        row == request.second ? ++FN : ++TN;
      }
    }
  }
  const double time = static_cast<double>(clock() - start) /
                      static_cast<double>(CLOCKS_PER_SEC),
               average_accuracy = (TP + TN) / (TP + FP + FN + TN),
               precision = TP / (TP + FP), recall = TP / (TP + FN),
               f_measure = (2.f * precision * recall) / (precision + recall);
  return TestOutput{average_accuracy, precision, recall, f_measure, time};
}

}  // namespace s21
