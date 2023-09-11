#include "base_network.h"

std::size_t s21::BaseNetwork::GetRightIndex(const Matrix<float> &last_layer) {
  std::size_t max_index = 0;
  float max = -1;
  for (std::size_t index = 0; index < last_layer.GetRows(); ++index) {
    const float answer = last_layer(index, 0);
    if (answer > max) {
      max = answer;
      max_index = index;
    }
  }
  return max_index;
}

double s21::BaseNetwork::GetLastMse() const { return mse; }
