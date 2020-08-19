#include "Matrix.h"

#include <stdlib.h>
typedef struct Matrix {
  uint32_t height;
  uint32_t width;
  double **values;
} Matrix;

ErrorCode matrix_create(PMatrix *const matrix, const uint32_t height,
                        const uint32_t width) {
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

  mat->height = height;
  mat->width = width;

  mat->values = (double **)malloc(height * sizeof(double *));
  if (mat->values == NULL) {
    free(mat);
    return ERROR_ALLOCATION_FAILED;
  }
  for (uint32_t i = 0; i < mat->height; ++i) {
    mat->values[i] = (double *)calloc(width, sizeof(double));
    if (mat->values[i] == NULL) {
      for (uint32_t j = 0; j < i; ++j) {
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

ErrorCode matrix_copy(PMatrix *const result, CPMatrix const source) {
  if (source == NULL || result == NULL) {
    return ERROR_NULL;
  }

  ErrorCode code = matrix_create(result, source->height, source->width);
  if (!error_isSuccess(code)) {
    return code;
  }

  for (uint32_t i = 0; i < (*result)->height; ++i) {
    for (uint32_t j = 0; j < (*result)->width; ++j) {
      (*result)->values[i][j] = source->values[i][j];
    }
  }

  return ERROR_SUCCESS;
}

void matrix_destroy(PMatrix const matrix) {
  if (matrix != NULL) {
    for (uint32_t i = 0; i < matrix->height; ++i) {
      free(matrix->values[i]);
    }
    free(matrix->values);
    free(matrix);
  }
}

ErrorCode matrix_getHeight(CPMatrix const matrix, uint32_t *const result) {
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  *result = matrix->height;
  return ERROR_SUCCESS;
}

ErrorCode matrix_getWidth(CPMatrix const matrix, uint32_t *const result) {
  if (matrix == NULL || result == NULL) {
    return ERROR_NULL;
  }

  *result = matrix->width;
  return ERROR_SUCCESS;
}

ErrorCode matrix_setValue(PMatrix const matrix, const uint32_t rowIndex,
                          const uint32_t colIndex, const double value) {
  if (matrix == NULL) {
    return ERROR_NULL;
  }
  if (matrix->height <= rowIndex || matrix->width <= colIndex) {
    return ERROR_OUT_OF_BOUNDS;
  }

  matrix->values[rowIndex][colIndex] = value;
  return ERROR_SUCCESS;
}

ErrorCode matrix_getValue(CPMatrix const matrix, const uint32_t rowIndex,
                          const uint32_t colIndex, double *const value) {
  if (matrix == NULL || value == NULL) {
    return ERROR_NULL;
  }

  if (matrix->height <= rowIndex || matrix->width <= colIndex) {
    return ERROR_OUT_OF_BOUNDS;
  }

  *value = matrix->values[rowIndex][colIndex];
  return ERROR_SUCCESS;
}

ErrorCode matrix_add(PMatrix *const result, CPMatrix const lhs,
                     CPMatrix const rhs) {
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
  for (uint32_t i = 0; i < lhs->height; ++i) {
    for (uint32_t j = 0; j < lhs->width; ++j) {
      matrix->values[i][j] = lhs->values[i][j] + rhs->values[i][j];
    }
  }
  return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyMatrices(PMatrix *const result, CPMatrix const lhs,
                                  CPMatrix const rhs) {
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
  for (uint32_t i = 0; i < matrix->height; ++i) {
    for (uint32_t j = 0; j < matrix->width; ++j) {
      double val = 0;
      for (uint32_t k = 0; k < lhs->width; ++k) {
        val += lhs->values[i][k] * rhs->values[k][j];
      }
      matrix->values[i][j] = val;
    }
  }
  return ERROR_SUCCESS;
}

ErrorCode matrix_multiplyWithScalar(PMatrix const matrix, const double scalar) {
  if (matrix == NULL) {
    return ERROR_NULL;
  }

  for (uint32_t i = 0; i < matrix->height; ++i) {
    for (uint32_t j = 0; j < matrix->width; ++j) {
      matrix->values[i][j] *= scalar;
    }
  }
  return ERROR_SUCCESS;
}