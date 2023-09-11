#include <gtest/gtest.h>

#include "test.h"

namespace {
const std::string path_weights = "sample/weight_for_test.net";
} // namespace

TEST(ForwardFeed, LoadPlusLetterA) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_a()));
}

TEST(ForwardFeed, LoadPlusLetterI) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_i()));
}

TEST(ForwardFeed, LoadPlusLetterL) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_r()));
}

TEST(ForwardFeed, LoadPlusLetterP) {
  ::s21::Model model;
  model.LoadWeights(path_weights);
  EXPECT_TRUE(::test::TestLetter(model, ::test::letter_p()));
}
