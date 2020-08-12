#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  // DON'T CHANGE THIS VALUE!
  ERROR_SUCCESS = 0,
  ERROR_NULL = 1,
  ERROR_ALLOCATION_FAILED = 2,
  ERROR_OUT_OF_BOUNDS = 3,
  ERROR_SIZES_NOT_MATCH = 4
} ErrorCode;

/**
 * @brief Checks if a given error code indicates a success or not.
 *
 * @param[in] code the error code.
 * @return whether the error code indicates a success or not.
 */
bool error_isSuccess(ErrorCode code);

/**
 * @brief gets a textual error message for a given error code.
 *
 * @param[in] code the error code.
 * @return const char* the textual representation of the error code.
 */
const char *error_getErrorMessage(ErrorCode code);