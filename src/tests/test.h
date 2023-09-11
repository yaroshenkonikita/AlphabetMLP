#include "../model/model.h"

namespace test {

enum class Alphabet : uint32_t {
  A = 0,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z
};

std::pair<Alphabet, std::vector<float>> letter_a();

std::pair<Alphabet, std::vector<float>> letter_i();

std::pair<Alphabet, std::vector<float>> letter_r();

std::pair<Alphabet, std::vector<float>> letter_p();

bool TestLetter(s21::Model &model,
                std::pair<Alphabet, std::vector<float>> answer_sensors);

bool CompareFiles(const std::string &l_file_path,
                  const std::string &r_file_path);

}  // namespace test