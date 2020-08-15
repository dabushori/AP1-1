#include "Matrix.h"
#include <stdlib.h>

// The implementation of the struct is up to you
typedef struct Matrix {
  int height;
  int width;
  double **values;
} Matrix;

/**
 * @brief Creates a new matrix of a given height an width,
 *  all values are initially zeroes.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the created matrix.
 * @param[in] height Height of the matrix
 * @param[in] width Width of the matrix
 * @return ErrorCode
 */
ErrorCode matrix_create(PMatrix *matrix, uint32_t height, uint32_t width) {
  if (matrix == NULL) {
    return ERROR_NULL;
  }

  if (height == 0 || width == 0) {
    return ERROR_SIZES_NOT_MATCH;
  }

  PMatrix mat = (PMatrix)malloc(sizeof(Matrix));
  if (mat == NULL) {
    return ERROR_ALLOCATION_FAILED;
  }

  // setting the sizes of the matrix
  mat->height = height;
  mat->width = width;

  // creating the 2D-array
  mat->values = (double **)malloc(height * sizeof(double *));
  if (mat->values == NULL) {
    free(mat);
    return ERROR_ALLOCATION_FAILED;
  }
  for (int i = 0; i < mat->height; i++) {
    mat->values[i] = (double *)calloc(width, sizeof(double));
    if (mat->values[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(mat->values[j]);
      }
      free(mat->values);
      free(mat);
      return ERROR_ALLOCATION_FAILED;
    }
  }

  *matrix = mat;
  return ERROR_SUCCESS;
}

/**
 * @brief Creates a new matrix from an old matrix.
 *
 * @param[out] matrix The address of a matrix pointer to receive
 *  the address of the copied matrix.
 * @param[in] source The matrix to copy.
 * @return ErrorCode
 */
ErrorCode matrix_copy(PMatrix *result, CPMatrix source) {
  if (source == NULL || result == NULL) {
    return ERROR_NULL;
  }

  uint32_t h = source->height, w = source->width;
  ErrorCode code =
      matrix_create(result, h, w);
  if (!error_isSuccess(code)) {
    return code;
  }

  for (int i = 0; i < (*result)->height; i++) {
    for (int j = 0; j < (*result)->width; j++) {
      (*result)->values[i][j] = source->values[i][j];
    }
  }

  return ERROR_SUCCESS;
}

/**
 * @brief Destroys a matrix.
 *
 * @param matrix the matrix to destroy.
 */
void matrix_destroy(PMatrix matrix) {
  if (matrix != NULL) {
    for (int i = 0; i < matrix->height; i++) {
      free(matrix->values[i]);
    }
    free(matrix->values);
    free(matrix);
  }
}

/**
 * @brief Returns the height of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getHeight(CPMatrix matrix, uint32_t *result) {
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  *result = matrix->height;
  return ERROR_SUCCESS;
}

/**
 * @brief Returns the width of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getWidth(CPMatrix matrix, uint32_t *result) {
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  *result = matrix->width;
  return ERROR_SUCCESS;
}

/**
 * @brief Sets a value to the matrix.
 *
 * @param[in, out] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to set.
 * @param[in] colIndex Column index of the value to setF.
 * @param[in] value Value to set.
 * @return ErrorCode
 */
ErrorCode matrix_setValue(PMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double value) {
  if (matrix == NULL) {
    return ERROR_NULL;
  }
  int row = (int)rowIndex, col = (int)colIndex;
  if (matrix->height <= row || matrix->width <= col) {
    return ERROR_OUT_OF_BOUNDS;
  }

  matrix->values[row][col] = value;
  return ERROR_SUCCESS;
}

/**
 * @brief Sets a value to the matrix.
 *
 * @param[in] matrix The matrix to operate on.
 * @param[in] rowIndex Row index of the value to get.
 * @param[in] colIndex Column index of the value to get.
 * @param[out] value The address of a double variable to receive
 *  the value from the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getValue(CPMatrix matrix, uint32_t rowIndex, uint32_t colIndex,
                          double *value) {
  if (matrix == NULL || value == NULL) {
    return ERROR_NULL;
  }
  int row = (int)rowIndex, col = (int)colIndex;
  if (matrix->height <= row || matrix->width <= col) {
    return ERROR_OUT_OF_BOUNDS;
  }

  *value = matrix->values[row][col];
  return ERROR_SUCCESS;
}

/**
 * @brief Computes the addition of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the addition operation.
 * @param[in] rhs The right hand side of the addition operation.
 * @return ErrorCode
 */
ErrorCode matrix_add(PMatrix *result, CPMatrix lhs, CPMatrix rhs) {
  if (lhs == NULL || rhs == NULL) {
    return ERROR_NULL;
  }
  if (lhs->height != rhs->height || lhs->width != rhs->width) {
    return ERROR_SIZES_NOT_MATCH;
  }

  ErrorCode code = matrix_create(result, lhs->height, lhs->width);
  if (!error_isSuccess(code)) {
    return code;
  }

  PMatrix matrix = *result;
  for (int i = 0; i < lhs->height; i++) {
    for (int j = 0; j < lhs->width; j++) {
      matrix->values[i][j] = lhs->values[i][j] + rhs->values[i][j];
    }
  }
  return ERROR_SUCCESS;
}

/**
 * @brief Computes the multiplication of two matrices.
 *
 * @param[out] result The address of a matrix pointer to receive
 *  the address of the resulting matrix.
 * @param[in] lhs The left hand side of the multiplication operation.
 * @param[in] rhs The right hand side of the multiplication operation.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyMatrices(PMatrix *result, CPMatrix lhs, CPMatrix rhs) {
  if (lhs == NULL || rhs == NULL) {
    return ERROR_NULL;
  }
  if (lhs->width != rhs->height) {
    return ERROR_SIZES_NOT_MATCH;
  }

  ErrorCode code = matrix_create(result, lhs->height, rhs->width);
  if (!error_isSuccess(code)) {
    return code;
  }

  PMatrix matrix = *result;
  for (int i = 0; i < matrix->height; i++) {
    for (int j = 0; j < matrix->width; j++) {
      double val = 0;
      for (int k = 0; k < lhs->width; k++) {
        val += lhs->values[i][k] * rhs->values[k][j];
      }
      matrix->values[i][j] = val;
    }
  }
  return ERROR_SUCCESS;
}

/**
 * @brief Multiplies a matrix with a scalar and stores the result in
 *  the given matrix.
 *
 * @param[in, out] matrix On input, the matrix to multiply with a scalar.
 *  On output, the result of the multiplication operation.
 * @param[in] scalar The scalar to multiply with.
 * @return ErrorCode
 */
ErrorCode matrix_multiplyWithScalar(PMatrix matrix, double scalar) {
  if (matrix == NULL) {
    return ERROR_NULL;
  }
  for (int i = 0; i < matrix->height; i++) {
    for (int j = 0; j < matrix->width; j++) {
      matrix->values[i][j] *= scalar;
    }
  }
  return ERROR_SUCCESS;
}