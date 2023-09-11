#include <gtest/gtest.h>

#include "test.h"

namespace {
const std::string train_sample = "sample/train_for_test.csv";
} // namespace

TEST(Reader, OpenFile) {
    ::s21::ReaderEMNIST reader;
    reader.OpenFile(train_sample);
    EXPECT_EQ(reader.Size(), 79);
    auto sample = reader.GetVector();
    EXPECT_EQ(sample.size(), reader.Size());
    for (const auto &line : sample) {
        EXPECT_TRUE(line.second >= static_cast<std::size_t>(::test::Alphabet::A));
        EXPECT_TRUE(line.second <= static_cast<std::size_t>(::test::Alphabet::Z));
        EXPECT_EQ(line.first.GetRows(), 784);
        EXPECT_EQ(line.first.GetColumns(), 1);
    }
}
