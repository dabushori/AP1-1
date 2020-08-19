#include "Matrix.h"

#include <stdlib.h>

// The implementation of the Matrix struct
typedef struct Matrix {
  uint32_t height;
  uint32_t width;
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
ErrorCode matrix_create(PMatrix *const matrix, const uint32_t height,
                        const uint32_t width) {
  // checking that the given ptr is not NULL
  if (matrix == NULL) {
    return ERROR_NULL;
  }

  // checking that the size of the matrix is not 0
  if (height == 0 || width == 0) {
    return ERROR_SIZES_NOT_MATCH;
  }

  // alocating the matrix 'object'
  PMatrix mat = (PMatrix)malloc(sizeof(Matrix));
  // checking that the allocation was successfull
  if (mat == NULL) {
    return ERROR_ALLOCATION_FAILED;
  }

  // setting the size of the matrix
  mat->height = height;
  mat->width = width;

  // creating the 2D-array
  mat->values = (double **)malloc(height * sizeof(double *));
  // checking that the allocation was successfull
  if (mat->values == NULL) {
    free(mat);
    return ERROR_ALLOCATION_FAILED;
  }
  for (uint32_t i = 0; i < mat->height; ++i) {
    mat->values[i] = (double *)calloc(width, sizeof(double));
    // checking that the allocation was successfull
    if (mat->values[i] == NULL) {
      for (uint32_t j = 0; j < i; ++j) {
        free(mat->values[j]);
      }
      free(mat->values);
      free(mat);
      return ERROR_ALLOCATION_FAILED;
    }
  }

  // setting the given adress to be the matrix we created
  *matrix = mat;
  // returning success error-code
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
ErrorCode matrix_copy(PMatrix *const result, CPMatrix const source) {
  // checking that the given ptrs is not NULL
  if (source == NULL || result == NULL) {
    return ERROR_NULL;
  }

  // creating the result matrix
  ErrorCode code = matrix_create(result, source->height, source->width);
  // checking that the creation was successfull
  if (!error_isSuccess(code)) {
    return code;
  }

  // copying the values
  for (uint32_t i = 0; i < (*result)->height; ++i) {
    for (uint32_t j = 0; j < (*result)->width; ++j) {
      (*result)->values[i][j] = source->values[i][j];
    }
  }

  // returning success error-code
  return ERROR_SUCCESS;
}

/**
 * @brief Destroys a matrix.
 *
 * @param matrix the matrix to destroy.
 */
void matrix_destroy(PMatrix const matrix) {
  // checking that the given ptr is not NULL
  if (matrix != NULL) {
    // freeing the 2D array
    for (uint32_t i = 0; i < matrix->height; ++i) {
      free(matrix->values[i]);
    }
    free(matrix->values);
    // freeing the matrix itself
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
ErrorCode matrix_getHeight(CPMatrix const matrix, uint32_t *const result) {
  // checking that the given ptrs is not NULL
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  // getting the matrix's height
  *result = matrix->height;
  // returning success error-code
  return ERROR_SUCCESS;
}

/**
 * @brief Returns the width of a give matrix.
 *
 * @param[in] matrix The matrix.
 * @param[out] result On output, contains the height of the matrix.
 * @return ErrorCode
 */
ErrorCode matrix_getWidth(CPMatrix const matrix, uint32_t *const result) {
  // checking that the given ptrs is not NULL
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  // getting the matrix's width
  *result = matrix->width;
  // returning success error-code
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
ErrorCode matrix_setValue(PMatrix const matrix, const uint32_t rowIndex,
                          const uint32_t colIndex, const double value) {
  // checking that the given ptr is not NULL
  if (matrix == NULL) {
    return ERROR_NULL;
  }
  // checking that the given indexes are valid
  if (matrix->height <= rowIndex || matrix->width <= colIndex) {
    return ERROR_OUT_OF_BOUNDS;
  }

  // setting the wanted value
  matrix->values[rowIndex][colIndex] = value;
  // returning success error-code
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
ErrorCode matrix_getValue(CPMatrix const matrix, const uint32_t rowIndex,
                          const uint32_t colIndex, double *const value) {
  // checking that the given ptrs are not NULL
  if (matrix == NULL || value == NULL) {
    return ERROR_NULL;
  }

  // checking that the given indexes are valid
  if (matrix->height <= rowIndex || matrix->width <= colIndex) {
    return ERROR_OUT_OF_BOUNDS;
  }

  // getting the wanted value
  *value = matrix->values[rowIndex][colIndex];
  // returning success error-code
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
ErrorCode matrix_add(PMatrix *const result, CPMatrix const lhs,
                     CPMatrix const rhs) {
  // checking that the given ptrs are nut NULL
  if (lhs == NULL || rhs == NULL) {
    return ERROR_NULL;
  }

  // checking that the sizes of the matrices match
  if (lhs->height != rhs->height || lhs->width != rhs->width) {
    return ERROR_SIZES_NOT_MATCH;
  }

  // creating the result matrix
  ErrorCode code = matrix_create(result, lhs->height, lhs->width);
  // checking if the creation was successfull
  if (!error_isSuccess(code)) {
    return code;
  }

  PMatrix matrix = *result;
  // adding the values
  for (uint32_t i = 0; i < lhs->height; ++i) {
    for (uint32_t j = 0; j < lhs->width; ++j) {
      matrix->values[i][j] = lhs->values[i][j] + rhs->values[i][j];
    }
  }
  // returning success error-code
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
ErrorCode matrix_multiplyMatrices(PMatrix *const result, CPMatrix const lhs,
                                  CPMatrix const rhs) {
  // checking that the given ptrs are nut NULL
  if (lhs == NULL || rhs == NULL) {
    return ERROR_NULL;
  }

  // checking that the sizes of the matrices match
  if (lhs->width != rhs->height) {
    return ERROR_SIZES_NOT_MATCH;
  }

  // creating the result matrix
  ErrorCode code = matrix_create(result, lhs->height, rhs->width);
  // checking that the creation was successfull
  if (!error_isSuccess(code)) {
    return code;
  }

  PMatrix matrix = *result;
  // multiplying the matrices
  for (uint32_t i = 0; i < matrix->height; ++i) {
    for (uint32_t j = 0; j < matrix->width; ++j) {
      double val = 0;
      for (uint32_t k = 0; k < lhs->width; ++k) {
        val += lhs->values[i][k] * rhs->values[k][j];
      }
      matrix->values[i][j] = val;
    }
  }
  // returning success error-code
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
ErrorCode matrix_multiplyWithScalar(PMatrix const matrix, const double scalar) {
  // checking that the given ptr is not NULL
  if (matrix == NULL) {
    return ERROR_NULL;
  }

  // multiplying the values
  for (uint32_t i = 0; i < matrix->height; ++i) {
    for (uint32_t j = 0; j < matrix->width; ++j) {
      matrix->values[i][j] *= scalar;
    }
  }
  // returning success error-code
  return ERROR_SUCCESS;
}