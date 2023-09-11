#include <gtest/gtest.h>

#include "test.h"

namespace {
const std::string path_weights = "sample/weight_for_test.net";
} // namespace

TEST(LoadWeights, Matrix) {
  ::s21::Model model;
  model.SetMatrixNetwork();
  model.LoadWeights(path_weights);
  EXPECT_EQ(model.GetCountLayers(), 5);
  EXPECT_EQ(model.GetCountNeurons(), 64);
  EXPECT_TRUE(model.IsMatrixNetwork());
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_r()));
}

TEST(LoadWeights, Graph) {
  ::s21::Model model;
  model.SetGraphNetwork();
  model.LoadWeights(path_weights);
  EXPECT_EQ(model.GetCountLayers(), 5);
  EXPECT_EQ(model.GetCountNeurons(), 64);
  EXPECT_TRUE(model.IsGraphNetwork());
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_p()));
}
