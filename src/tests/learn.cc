#include <gtest/gtest.h>

#include "test.h"

namespace {
const std::string path_weights = "sample/weight_for_test.net";
const std::string train_sample = "sample/train_for_test.csv";
const std::string tmp_learn_path = "tmp_learn.net";
const std::string weight_after_learn_path = "sample/weight_after_learn.net";
} // namespace

TEST(Learn, LoadReaderSave) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  ::s21::ReaderEMNIST train(train_sample);
  model.Learn(train);
  model.SaveWeights(tmp_learn_path);
  EXPECT_TRUE(::test::CompareFiles(tmp_learn_path, weight_after_learn_path));
}
