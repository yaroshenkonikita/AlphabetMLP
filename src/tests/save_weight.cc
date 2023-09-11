#include <gtest/gtest.h>

#include "test.h"

namespace {
const std::string path_weights = "sample/weight_for_test.net";
const std::string tmp_save_path = "tmp_save.net";
} // namespace

TEST(SaveWeights, LoadSave) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  model.SaveWeights(tmp_save_path);
  EXPECT_TRUE(::test::CompareFiles(tmp_save_path, path_weights));
}
