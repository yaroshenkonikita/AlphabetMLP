#pragma once

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

namespace s21 {

template <class T>
class Matrix {
  static_assert(std::is_arithmetic<T>(),
                "Matrix template type must be arithmetic");

 public:
  Matrix();
  Matrix(std::size_t rows, std::size_t columns);
  Matrix(const Matrix &other);
  Matrix(Matrix &&other);

  Matrix &operator=(const Matrix &other);
  Matrix &operator=(Matrix &&other);

  Matrix &operator*=(T value);
  Matrix &operator*=(const Matrix &other);
  Matrix &operator+=(const Matrix &other);
  Matrix &operator-=(const Matrix &other);

  ~Matrix();

  T &operator()(std::size_t rows, std::size_t columns) const;

  std::size_t GetColumns() const noexcept;
  std::size_t GetRows() const noexcept;
  std::size_t Size() const noexcept;

  void Set(std::size_t rows, std::size_t columns);
  void SetRows(std::size_t rows);
  void SetColumns(std::size_t columns);

  bool EqMatrix(const Matrix &other) const;

  void SumMatrix(const Matrix &other);
  void SubMatrix(const Matrix &other);
  void MulNumber(T value);
  void MulMatrix(const Matrix &other);

  Matrix Transpose() const;
  Matrix MinorMatrix(std::size_t discard_row, std::size_t discard_column) const;
  Matrix CalcComplements() const;
  T Determinant() const;
  Matrix InverseMatrix() const;

 private:
  void CheckEqSize(const Matrix &other) const;
  void AllocMemory();

  std::size_t rows_, columns_;
  T **matrix_;
};

template <class T>
std::ostream &operator<<(std::ostream &out, const Matrix<T> matrix) {
  if (out.bad()) {
    return out;
  }
  out << matrix.GetRows() << ' ' << matrix.GetColumns() << '\n';
  for (std::size_t i = 0; i < matrix.GetRows(); ++i) {
    for (std::size_t j = 0; j < matrix.GetColumns(); ++j) {
      out << matrix(i, j) << ((j + 1) == matrix.GetColumns() ? '\n' : ' ');
    }
  }
  return out;
}

template <class T>
std::istream &operator>>(std::istream &in, Matrix<T> &matrix) {
  if (in.bad()) {
    return in;
  }
  std::size_t rows, cols;
  in >> rows >> cols;
  if (in.bad()) {
    return in;
  }
  Matrix<T> tmp{rows, cols};
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < cols; ++j) {
      in >> tmp(i, j);
      if (in.bad()) {
        return in;
      }
    }
  }
  matrix = std::move(tmp);
  return in;
}

template <class T>
Matrix<T>::Matrix() : rows_(3), columns_(3) {
  AllocMemory();
}

template <class T>
Matrix<T>::Matrix(const std::size_t rows, const std::size_t columns)
    : rows_(rows), columns_(columns) {
  if (!rows_ || !columns_) {
    throw std::invalid_argument("Arguments cannot be zero");
  }
  AllocMemory();
}

template <class T>
Matrix<T>::Matrix(const Matrix<T> &other)
    : rows_(other.rows_), columns_(other.columns_) {
  AllocMemory();
  std::copy_n(other.matrix_[0], Size(), matrix_[0]);
}

template <class T>
Matrix<T>::Matrix(Matrix<T> &&other)
    : rows_(std::exchange(other.rows_, 3)),
      columns_(std::exchange(other.columns_, 3)),
      matrix_(std::exchange(other.matrix_, nullptr)) {
  other.AllocMemory();
}

template <class T>
Matrix<T>::~Matrix() {
  delete[] matrix_[0];
  delete[] matrix_;
}

template <class T>
std::size_t Matrix<T>::GetColumns() const noexcept {
  return columns_;
}

template <class T>
std::size_t Matrix<T>::GetRows() const noexcept {
  return rows_;
}

template <class T>
std::size_t Matrix<T>::Size() const noexcept {
  return columns_ * rows_;
}

template <class T>
void Matrix<T>::Set(const std::size_t rows, const std::size_t columns) {
  Matrix<T> temp(rows, columns);
  for (std::size_t row = 0; row < std::min(rows_, rows); ++row) {
    for (std::size_t column = 0; column < std::min(columns_, columns);
         ++column) {
      temp.matrix_[row][column] = matrix_[row][column];
    }
  }
  *this = std::move(temp);
}

template <class T>
void Matrix<T>::SetRows(const std::size_t rows) {
  Set(rows, columns_);
}

template <class T>
void Matrix<T>::SetColumns(const std::size_t columns) {
  Set(rows_, columns);
}

template <class T>
T &Matrix<T>::operator()(const std::size_t rows,
                         const std::size_t columns) const {
  if (rows >= rows_ || columns >= columns_) {
    throw std::out_of_range("Index out of range");
  }
  return matrix_[rows][columns];
}

template <class T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &other) {
  return SumMatrix(other), *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &other) {
  return MulMatrix(other), *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator*=(const T value) {
  return MulNumber(value), *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &other) {
  return SubMatrix(other), *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &other) {
  if (this != &other) {
    Matrix<T> temp(other);
    *this = std::move(temp);
  }
  return *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&other) {
  if (this != &other) {
    this->~Matrix();
    rows_ = std::exchange(other.rows_, 3);
    columns_ = std::exchange(other.columns_, 3);
    matrix_ = std::exchange(other.matrix_, nullptr);
    other.AllocMemory();
  }
  return *this;
}

template <class T>
bool Matrix<T>::EqMatrix(const Matrix<T> &other) const {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    return false;
  }
  for (std::uint64_t i = 0, max_index = Size(); i < max_index; ++i) {
    if (matrix_[0][i] != other.matrix_[0][i]) {
      return false;
    }
  }
  return true;
}

template <class T>
void Matrix<T>::SumMatrix(const Matrix<T> &other) {
  CheckEqSize(other);
  for (std::uint64_t i = 0, max_index = Size(); i < max_index; ++i) {
    matrix_[0][i] += other.matrix_[0][i];
  }
}

template <class T>
void Matrix<T>::SubMatrix(const Matrix<T> &other) {
  CheckEqSize(other);
  for (std::uint64_t i = 0, max_index = Size(); i < max_index; ++i) {
    matrix_[0][i] -= other.matrix_[0][i];
  }
}

template <class T>
void Matrix<T>::MulNumber(const T value) {
  for (std::uint64_t i = 0, max_index = Size(); i < max_index; ++i) {
    matrix_[0][i] *= value;
  }
}

template <class T>
void Matrix<T>::MulMatrix(const Matrix<T> &other) {
  if (columns_ != other.rows_) {
    throw std::invalid_argument(
        "Wrong matrix, different Size first matrix "
        "columns and second matrix rows");
  }

  const std::size_t d = columns_ / 2;
  std::vector<T> rowFactor(rows_);
  std::vector<T> colsFactor(other.columns_);
  for (std::size_t i = 0; i < rows_; ++i) {
    rowFactor[i] = matrix_[i][0] * matrix_[i][1];
    for (std::size_t j = 1; j < d; ++j) {
      const std::size_t mul = 2 * j;
      rowFactor[i] += matrix_[i][mul] * matrix_[i][mul + 1];
    }
  }
  for (std::size_t i = 0; i < other.columns_; ++i) {
    colsFactor[i] = other(0, i) * other(1, i);
    for (std::size_t j = 1; j < d; ++j) {
      const std::size_t mul = 2 * j;
      colsFactor[i] += other.matrix_[mul][i] * other.matrix_[mul + 1][i];
    }
  }

  Matrix<T> result{rows_, other.columns_};

  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < other.columns_; ++j) {
      result(i, j) = -rowFactor[i] - colsFactor[j];
      for (std::size_t k = 0; k < d; ++k) {
        const std::size_t mul = 2 * k;
        result.matrix_[i][j] += (matrix_[i][mul] + other.matrix_[mul + 1][j]) *
                                (matrix_[i][mul + 1] + other.matrix_[mul][j]);
      }
    }
  }

  if (columns_ % 2 == 1) {
    for (std::size_t i = 0; i < rows_; ++i) {
      for (std::size_t j = 0; j < other.columns_; ++j) {
        result.matrix_[i][j] +=
            matrix_[i][columns_ - 1] * other.matrix_[columns_ - 1][j];
      }
    }
  }

  *this = std::move(result);
}

template <class T>
[[nodiscard]] Matrix<T> Matrix<T>::Transpose() const {
  Matrix<T> result(columns_, rows_);
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < columns_; ++j) {
      result(j, i) = (*this)(i, j);
    }
  }
  return result;
}

template <class T>
[[nodiscard]] Matrix<T> Matrix<T>::MinorMatrix(
    const std::size_t discard_row, const std::size_t discard_column) const {
  if (rows_ == 1 || columns_ == 1 || discard_row >= rows_ ||
      discard_column >= columns_) {
    throw std::invalid_argument("Invalid argument, impossible minor matrix");
  }
  Matrix<T> result(rows_ - 1, columns_ - 1);
  for (std::size_t i = 0, index_row = 0; i < rows_; ++i) {
    for (std::size_t j = 0, index_column = 0; j < columns_; ++j) {
      if (i == discard_row || j == discard_column) continue;
      result(index_row, index_column++) = matrix_[i][j];
    }
    if (i != discard_row) {
      index_row++;
    }
  }
  return result;
}

template <class T>
[[nodiscard]] Matrix<T> Matrix<T>::CalcComplements() const {
  if (rows_ != columns_ || rows_ < 2) {
    throw std::invalid_argument(
        "Impossible calc complements, matrix is not square");
  }
  Matrix<T> result(rows_, columns_);
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < columns_; ++j) {
      result.matrix_[i][j] =
          MinorMatrix(i, j).Determinant() * (((i + j) % 2) == 0 ? 1 : -1);
    }
  }
  return result;
}

template <class T>
T Matrix<T>::Determinant() const {
  if (rows_ != columns_) {
    throw std::invalid_argument(
        "Impossible determinant matrix, matrix is not square");
  }
  if (rows_ == 1) {
    return matrix_[0][0];
  } else if (rows_ == 2) {
    return matrix_[0][0] * matrix_[0][3] - matrix_[0][1] * matrix_[0][2];
  }
  T determinant = 0.0;
  for (std::size_t j = 0; j < rows_; ++j) {
    determinant += matrix_[0][j] * MinorMatrix(0, j).Determinant() *
                   ((j % 2) == 0 ? 1 : -1);
  }
  return determinant;
}

template <class T>
[[nodiscard]] Matrix<T> Matrix<T>::InverseMatrix() const {
  T det = Determinant();
  if (fabs(det) < 1e-6) {
    throw std::invalid_argument("Impossible inverse matrix, determinant is 0");
  }
  Matrix<T> temp = CalcComplements().Transpose();
  return temp *= 1. / det;
}

template <class T>
void Matrix<T>::CheckEqSize(const Matrix &other) const {
  if (rows_ != other.rows_ || columns_ != other.columns_) {
    throw std::invalid_argument("Wrong matrix, different Size");
  }
}

template <class T>
void Matrix<T>::AllocMemory() {
  matrix_ = new T *[rows_] {};
  matrix_[0] = new T[Size()]{};
  for (std::size_t index = 1; index < rows_; ++index) {
    matrix_[index] = matrix_[index - 1] + columns_;
  }
}

template <class T>
[[nodiscard]] Matrix<T> operator-(Matrix<T> lhs, const Matrix<T> &rhs) {
  return lhs -= rhs;
}

template <class T>
[[nodiscard]] Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) {
  return lhs += rhs;
}

template <class T>
Matrix<T> operator*(const T value, Matrix<T> src) {
  return src *= value;
}

template <class T>
Matrix<T> operator*(Matrix<T> src, const T value) {
  return src *= value;
}

template <class T>
[[nodiscard]] Matrix<T> operator*(Matrix<T> lhs, const Matrix<T> &rhs) {
  return lhs *= rhs;
}

template <class T>
bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  return lhs.EqMatrix(rhs);
}

template <class T>
bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  return !lhs.EqMatrix(rhs);
}

}  // namespace s21
