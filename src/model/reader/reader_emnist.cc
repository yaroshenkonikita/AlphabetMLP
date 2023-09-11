#include "reader_emnist.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace s21 {

ReaderEMNIST::ReaderEMNIST(const std::string &path) { OpenFile(path); }

std::vector<ReaderEMNIST::EmnistValue> ReaderEMNIST::GetVector(
    std::size_t start, std::size_t end) const {
  end = std::min(end, lines_.size());
  if (end <= start) {
    return std::vector<EmnistValue>{};
  } else if (start == 0 && end == lines_.size()) {
    return lines_;
  }
  std::vector<EmnistValue> data(end - start);
  for (std::size_t index = 0; index < data.size(); ++index, ++start) {
    data[index] = lines_[start];
  }
  return data;
}

void ReaderEMNIST::OpenFile(const std::string &path) {
  lines_.clear();
  std::ifstream file{path};
  std::string line, num;
  while (getline(file, line)) {
    std::stringstream current(line);
    EmnistValue item(Matrix<float>(count_sensors, 1), 0);
    if (!getline(current, num, ',')) {
      continue;
    }
    item.second = std::stoi(num) - 1;
    std::size_t index = 0;
    while (getline(current, num, ',')) {
      item.first(index, 0) = stof(num) / 255.f;
      ++index;
    }
    if (count_sensors == index) {
      lines_.push_back(item);
    }
  }
}

ReaderEMNIST::EmnistValue ReaderEMNIST::operator[](std::size_t index) const {
  return lines_[index];
}

std::size_t ReaderEMNIST::Size() const { return lines_.size(); }

}  // namespace s21
